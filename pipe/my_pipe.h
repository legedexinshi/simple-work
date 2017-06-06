#ifndef MY_PIPE_LEG
#define MY_PIPE_LEG

#include <cstdio>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

namespace myPipe {

	class AutoDeleteFile {
		public:
		AutoDeleteFile() {}
		~AutoDeleteFile() {
			for (auto file: fileVec) 
				unlink(file.c_str());
		}
		std::vector<std::string> fileVec;
	};
	static AutoDeleteFile _;

	int pipe(int fd[2], std::string path = "") {
		std::srand(time(NULL));
		std::string fileName = "/tmp/my_pipe_tmp_" + std::to_string((int)rand());
		if (path != "") fileName = path;
		printf("%s\n", fileName.c_str());
		if ((fd[0] = open(fileName.c_str(), O_CREAT | O_RDONLY, 0666)) == -1) 
			return -1;
		_.fileVec.push_back(fileName);
		if ((fd[1] = open(fileName.c_str(), O_WRONLY, 0666)) == -1) 
			return -2;
		return 0;
	}

	int close(int fd) {
		return ::close(fd);
	}

	ssize_t write(int fd, const void *buf, size_t nbyte) {
		return ::write(fd, buf, nbyte);
	}

	ssize_t read(int fd, void *buf, size_t  nbyte) {
		return ::read(fd, buf, nbyte);
	}

}

#endif
