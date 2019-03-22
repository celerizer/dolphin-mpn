#ifndef MPN_DOWNLOADQUEUE_H
#define MPN_DOWNLOADQUEUE_H

#include <fstream>
#include <sstream>

#include <SFML/Network/Http.hpp>

/* Apparently this dependancy doesn't support SSL */
#define MPN_WEBSITE "http://www.mariopartynetplay.com/"

typedef struct mpn_download_t
{
  std::string LocalFile;
  std::string RemoteFile;
} mpn_download_t;

class MpnDownloadQueue
{
public:
  MpnDownloadQueue();

  bool downloadAll();
  bool pushBack(const std::string& Uri);

private:
  std::vector<mpn_download_t> m_Downloads;
  sf::Http m_Http;
};

extern MpnDownloadQueue DownloadQueue;

#endif
