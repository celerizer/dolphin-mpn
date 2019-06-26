#ifndef MPN_DOWNLOADQUEUE_H
#define MPN_DOWNLOADQUEUE_H

#include <fstream>
#include <sstream>

#include <SFML/Network/Http.hpp>

/* Apparently this dependancy doesn't support SSL */
#define MPN_WEBSITE "http://www.mariopartynetplay.com/"
#define MPN_NO_UPDATES "MPN-OK"
#define MPN_VERSION 1

typedef struct mpn_download_t
{
  std::string LocalFile;
  std::string LocalPath;
  std::string RemoteFile;
} mpn_download_t;

class MpnDownloadQueue
{
public:
  MpnDownloadQueue();

  bool checkVersion();
  bool downloadAll();
  bool pushBack(const std::string& Uri);

private:
  std::vector<mpn_download_t> m_Downloads;
  sf::Http m_Http;
};

extern MpnDownloadQueue DownloadQueue;

#endif
