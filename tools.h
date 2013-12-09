#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <sys/types.h>
#include <pcap.h>

#include "gzstream/gzstream.h"

using namespace std;

int getUpdDifTime(struct timeval *before);
int getDifTime(struct timeval *before);
int msleep(long msec);
int file_exists (char * fileName);
void set_mac();
int mkdir_r(std::string, mode_t);
int rmdir_r(const char *dir, bool enableSubdir = false, bool withoutRemoveRoot = false);
unsigned long long cp_r(const char *src, const char *dst, bool move = false);
inline unsigned long long mv_r(const char *src, const char *dst) { return(cp_r(src, dst, true)); }  
unsigned long long copy_file(const char *src, const char *dst, bool move = false);
inline unsigned long long move_file(const char *src, const char *dst) { return(copy_file(src, dst, true)); }
double ts2double(unsigned int sec, unsigned int usec);
unsigned long long GetFileSize(std::string filename);
unsigned long long GetFileSizeDU(std::string filename);
unsigned long long GetDU(unsigned long long fileSize);
string GetFileMD5(std::string filename);
bool FileExists(char *strFilename);
void ntoa(char *res, unsigned int addr);
string escapeshellR(string &);
time_t stringToTime(const char *timeStr);
unsigned int getNumberOfDayToNow(const char *date);
string getActDateTimeF();
int get_unix_tid(void);
unsigned long getUptime();


class CircularBuffer
{
public:
	 CircularBuffer(size_t capacity);
	 ~CircularBuffer();

	 size_t size() const { return size_; }
	 size_t capacity() const { return capacity_; }
	 // Return number of bytes written.
	 size_t write(const char *data, size_t bytes);
	 // Return number of bytes read.
	 size_t read(char *data, size_t bytes);

private:
	 size_t beg_index_, end_index_, size_, capacity_;
	 char *data_;
};

struct dstring
{
	dstring() {
	}
	dstring(std::string str1, std::string str2) {
		str[0] = str1;
		str[1] = str2;
	}
	std::string operator [] (int indexStr) {
		return(str[indexStr]);
	}
	bool operator == (const dstring& other) const { 
		return(this->str[0] == other.str[0] &&
		       this->str[1] == other.str[1]); 
	}
	std::string str[2];
};

struct d_u_int32_t
{
	d_u_int32_t(u_int32_t val1 = 0, u_int32_t val2 = 0) {
		val[0] = val1;
		val[1] = val2;
	}
	u_int32_t operator [] (int indexVal) {
		return(val[indexVal]);
	}
	u_int32_t val[2];
};

struct ip_port
{
	ip_port() {
		port = 0;
	}
	ip_port(string ip, int port) {
		this->ip = ip;
		this->port = port;
	}
	void set_ip(string ip) {
		this->ip = ip;
	}
	void set_port(int port) {
		this->port = port;
	}
	string get_ip() {
		return(ip);
	}
	int get_port() {
		return(port);
	}
	operator int() {
		return(ip.length() && port);
	}
	std::string ip;
	int port;
};

inline u_long getTimeMS() {
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return(time.tv_sec * 1000 + time.tv_nsec / 1000000);
}

inline unsigned long long getTimeNS() {
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return(time.tv_sec * 1000000000ull + time.tv_nsec);
}

class PcapDumper {
public:
	enum eTypePcapDump {
		na,
		sip,
		rtp
	};
	PcapDumper(eTypePcapDump type, class Call *call, bool updateFilesQueueAtClose = true);
	~PcapDumper();
	bool open(const char *fileName, const char *fileNameSpoolRelative);
	void dump(pcap_pkthdr* header, const u_char *packet);
	void close(bool updateFilesQueue = true);
	void remove(bool updateFilesQueue = true);
	bool isOpen() {
		return(this->handle != NULL);
	}
private:
	string fileName;
	string fileNameSpoolRelative;
	eTypePcapDump type;
	class Call *call;
	bool updateFilesQueueAtClose;
	u_int64_t capsize;
	u_int64_t size;
	pcap_dumper_t *handle;
	bool openError;
	int openAttempts;
};

class RtpGraphSaver {
public:
	RtpGraphSaver(class RTP *rtp,bool updateFilesQueueAtClose = true);
	~RtpGraphSaver();
	bool open(const char *fileName, const char *fileNameSpoolRelative);
	void write(char *buffer, int length);
	void close(bool updateFilesQueue = true);
	bool isOpen() {
		extern int opt_gzipGRAPH;
		return(opt_gzipGRAPH ? this->streamgz.is_open() : this->stream.is_open());
	}
private:
	string fileName;
	string fileNameSpoolRelative;
	class RTP *rtp;
	bool updateFilesQueueAtClose;
	u_int64_t size;
	ofstream stream;
	ogzstream streamgz;
};

class RestartUpgrade {
public:
	RestartUpgrade(bool upgrade = false, const char *version = NULL, const char *url = NULL, const char *md5_32 = NULL, const char *md5_64 = NULL);
	bool runUpgrade();
	bool createRestartScript();
	bool checkReadyRestart();
	bool runRestart(int socket1, int socket2);
	bool isOk();
	string getErrorString();
	string getRsltString();
private:
	bool getUpgradeTempFileName();
	bool getRestartTempScriptFileName();
private:
	bool upgrade;
	string version;
	string url;
	string md5_32;
	string md5_64;
	string upgradeTempFileName;
	string restartTempScriptFileName;
	string errorString;
	bool _64bit;
};

#endif
