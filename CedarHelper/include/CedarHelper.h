#ifndef CEDAR_HELPER_H
#define CEDAR_HELPER_H

#include "json/json.h"
#include <signal.h>
#include <fstream>
#include <ctype.h>
#ifdef __linux
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <direct.h>
#endif
#include "CedarJsonConfig.h"
#include "CPlusPlusCode/ProtoBufMsg.pb.h"
#include "CmdLineParser.h"
#include "CedarLogger.h"

class CedarHelper {

public:
  static int cedarAppInit(int argc, char *argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    std::string configPath;
    configPath = CmdLineParser::getConfigPathFromCmd(argc, argv);
    CedarJsonConfig::getInstance().loadConfigFile(configPath);
    CedarLogger::init();
  }

  static int stringSplit(std::string str,
      char delimiter, std::vector<std::string> &res) {
    std::string token;
    std::istringstream ss(str);

    while (std::getline(ss, token, delimiter))
      res.push_back(token);

    return 0;
  }

  static std::string timestampString() {
    time_t sec;
    time(&sec);
    std::stringstream ss;
    ss << sec;
    return ss.str();
  }

  //this only get IPV4 addr, skip 127.0.0.1
  static std::string getResponseAddr() {
    std::string pull;
    CedarJsonConfig::getInstance().getStringByPath("MsgHub.PullPort", pull);
    std::string ipStr;
    if (getHostIP(ipStr) != 0)
      LOG(ERROR) << "can't get this machine IP address";
    return ipStr + ":" + pull;
  }

  static void getConfigRoot(std::string filepath, Json::Value& root) {
    std::ifstream config(filepath, std::ifstream::binary);
    Json::Reader reader;
    reader.parse(config, root, false);
  }

#ifdef __linux
  static int blockSignalAndSuspend() {
    int sig = 0, s = 0;
    sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet, SIGINT);
    sigaddset(&sigSet, SIGTERM);
    sigaddset(&sigSet, SIGKILL);
    sigprocmask(SIG_BLOCK, &sigSet, NULL);

    if ((s = sigwait(&sigSet, &sig)) != 0)
      perror("sigwait error\n");
    return 0;
  }

  //return current local time HHMMSSmmm
  static std::string getCurTimeStamp(){
    char currentTime[10];
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;
    char buffer [10];
    strftime(buffer, sizeof(buffer), "%H%M%S", localtime(&curTime.tv_sec));
    sprintf(currentTime, "%s%d", buffer, milli);

    return currentTime;
  }

  static std::string getOrderId() {
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    return std::to_string(curTime.tv_sec) + std::to_string(curTime.tv_usec);
  }
#endif

  static bool isStock(std::string code) {
    if (code.size() == 6 && isdigit(code[0]))
      return true;
    return false;
  }

private:
  static int getHostIP(std::string &ip) {
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void * tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
      if (!ifa->ifa_addr) {
        continue;
      }

      if (ifa->ifa_addr->sa_family == AF_INET) {
        // check it is IP4 is a valid IP4 Address
        tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
        char addressBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

        if (strcmp(addressBuffer ,"127.0.0.1") == 0) {
          continue;
        }

        ip = std::string(addressBuffer);

        if (ifAddrStruct != NULL)
          freeifaddrs(ifAddrStruct);

        return 0;
      }
    }
    return -1;
  }

};

#endif
