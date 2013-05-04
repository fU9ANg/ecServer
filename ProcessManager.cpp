
/**
 * @ingroup framework
 * @{
 */

#include "ProcessManager.h"

ProcessManager::ProcessManager()
{
    thrpool_ = new ThreadPool(10);
    lockfd_ = 0;
}

ProcessManager::~ProcessManager()
{
}

int ProcessManager::process_logic(int argc, char** argv)
{
  //设置信号处理
  signal(SIGINT, SIG_IGN);
  signal(SIGPIPE, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTERM, sig_term);

  int opt = getopt(argc, argv, "udsv");
  if ( -1 == opt) {
    printf("argv error\n");
    return -1;
  }
  int pid = 0;
  switch(opt){
    case 's':
      pid = fork();
      if(0 < pid){
        return 0;
      }
    case 'd':
      if(true != lock(LOCK_NOWAIT)) {
        printf("Process already running!\n");
        return 0;
      } 
      //初始化日志 
      assert( 0 == system("mkdir -p logs"));
      google::InitGoogleLogging(argv[0]);
      google::SetLogDestination(google::INFO, "./logs/info");
      google::SetLogDestination(google::WARNING, "./logs/warning");
      google::SetLogDestination(google::ERROR, "./logs/error");
      google::SetStderrLogging(google::ERROR + 1);
      //配置输出到标准错误的最低日志级别,使error日志不打屏
      CONFIG->readconfig(CONFIGFILE);
      if (!DATABASE->Init(CONFIG->db_host, CONFIG->db_username, CONFIG->db_password, CONFIG->db_database))
      {
          printf("Database error! Please check database status\n");
          return 0;
      }
      //启动
      this->run();
      break;
    case 'u':
      if (true == lock(LOCK_NOWAIT)) {   
        lock(UNLOCK);
        printf("No process!\n");
      } else {
        printf("Find process! stopping...\n");
        assert(0 == system("cat .lock|xargs kill -15"));
      }  
      printf("stop\n");
      break;
    case 'v':
      printf("compiled at %s,%s\n",__DATE__,__TIME__);
    default:
      break;
  }

  return 0;
}

void ProcessManager::sig_term(int signo)
{
  printf("get exit signal\n");
  LOG(INFO) << "server stoped";
  raise(SIGKILL);
  return;
}

/**
 *主循环
 */
int ProcessManager::run()
{
  printf("run\n");

  thrpool_->start();
  Evloop* evloop = new Evloop(CONFIG->server_ip.c_str(), CONFIG->server_port);
  printf("server_ip = [%s] port = [%d]\n", CONFIG->server_ip.c_str(), CONFIG->server_port);
  RecvTask* precv = new RecvTask();
  SendTask* psend = new SendTask();

  ROOMMANAGER->init();
  CHandleMessage::initHandlers();
  printf("[%d] class init success!\n", ROOMMANAGER->get_room_count());

  thrpool_->push_task(evloop);//监听和数据接收线程
  thrpool_->push_task(precv);//数据处理线程
  //thrpool_->push_task(precv);//数据处理线程
  //thrpool_->push_task(psend);
  thrpool_->push_task(psend);

  LOG(INFO) << "server start success!";

  //主线程死循环
  while (true) {
    //cout<<"new client count = "<<evloop->getClientCount()<<endl;
    LOG(INFO) << "new client count = " << evloop->getClientCount() << endl;
    sleep(5);
  }

  thrpool_->kill();
  return 0;
}

bool ProcessManager::lock(int mode)
{
  char szPid[32] = {0};
  snprintf(szPid, 32, "%ld", (long)getpid());
  if (0 == lockfd_) {
    lockfd_ = open("./.lock", O_RDWR|O_CREAT, 0640);
  }
  if (lockfd_ < 0) {
    return false;
  }
  if (0 != flock(lockfd_, mode)) {
    return false;
  }
  if (UNLOCK == mode) {
    close(lockfd_);
  }
  else {
    if ( 0 == ftruncate(lockfd_, 0) ) {
        if ( 0>= write(lockfd_, szPid, strlen(szPid))) {
            printf("write pid file error!\n");
        }
    }else {
        printf("write pid file error!\n");
        sleep(1);
        abort();
    }
  }
  return true;
}

///@}
