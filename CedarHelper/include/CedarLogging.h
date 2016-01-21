#ifndef CEDAR_LOGGING_H
#define CEDAR_LOGGING_H

#include "easylogging++.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

INITIALIZE_EASYLOGGINGPP

class CedarLogging {
public:

  static int init(std::string program) {
    el::Configurations defaultConf;
    defaultConf.setToDefault();

    defaultConf.setGlobally(
        el::ConfigurationType::Format, "%levshort%datetime{%M%d %H:%m:%s.%g} %thread %fbase:%line] %msg");

    std::string destDir = "./log/";
    defaultConf.set(el::Level::Info,    
        el::ConfigurationType::Filename, destDir + program + "_INFO_%datetime{%Y%M%d-%H%m%s.%g}");
    defaultConf.set(el::Level::Warning,    
        el::ConfigurationType::Filename, destDir + program + "_WARNING_%datetime{%Y%M%d-%H%m%s.%g}");
    defaultConf.set(el::Level::Error,    
        el::ConfigurationType::Filename, destDir + program + "_ERROR_%datetime{%Y%M%d-%H%m%s.%g}");
    defaultConf.set(el::Level::Fatal,   
        el::ConfigurationType::Filename, destDir + program + "_FATAL_%datetime{%Y%M%d-%H%m%s.%g}");

    el::Loggers::reconfigureLogger("default", defaultConf);

    el::Logger* l = el::Loggers::getLogger("default");
    int fd = open(destDir.c_str(), O_RDONLY);


    std::string filename = l->typedConfigurations()->filename(el::Level::Info);
    filename.erase(0, destDir.length());
    std::string symlinkFile = program + ".INFO";
    std::string oldSymLink = destDir + symlinkFile;
    remove(oldSymLink.c_str());
    if(symlinkat(filename.c_str(), fd, symlinkFile.c_str()) != 0)
      LOG(ERROR) << ".INFO symlink set fail!";

    filename = l->typedConfigurations()->filename(el::Level::Warning);
    filename.erase(0, destDir.length());
    symlinkFile = program + ".WARN";
    oldSymLink = destDir + symlinkFile;
    remove(oldSymLink.c_str());
    if(symlinkat(filename.c_str(), fd, symlinkFile.c_str()) != 0)
      LOG(ERROR) << ".WARN symlink set fail!";

    filename = l->typedConfigurations()->filename(el::Level::Error);
    filename.erase(0, destDir.length());
    symlinkFile = program + ".ERROR";
    oldSymLink = destDir + symlinkFile;
    remove(oldSymLink.c_str());
    if(symlinkat(filename.c_str(), fd, symlinkFile.c_str()) != 0)
      LOG(ERROR) << ".ERROR symlink set fail!";

    filename = l->typedConfigurations()->filename(el::Level::Fatal);
    filename.erase(0, destDir.length());
    symlinkFile = program + ".FATAL";
    oldSymLink = destDir + symlinkFile;
    remove(oldSymLink.c_str());
    if(symlinkat(filename.c_str(), fd, symlinkFile.c_str()) != 0)
      LOG(ERROR) << ".FATAL symlink set fail!";

    return 0;
  }
};

#endif
