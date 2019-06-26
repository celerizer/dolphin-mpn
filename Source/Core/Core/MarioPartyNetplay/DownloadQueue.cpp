#ifndef MPN_DOWNLOADQUEUE_CPP
#define MPN_DOWNLOADQUEUE_CPP

#include "Common/MsgHandler.h"

#include "DownloadQueue.h"
#include "windows.h"

MpnDownloadQueue DownloadQueue;

MpnDownloadQueue::MpnDownloadQueue()
{
  m_Downloads.clear();
}

bool MpnDownloadQueue::checkVersion()
{
  sf::Http::Request Request("/getUpdates.php", sf::Http::Request::Post, "v=" + MPN_VERSION);
  m_Http.setHost(MPN_WEBSITE);

  sf::Http::Response NeedsUpdate = m_Http.sendRequest(Request);
  /* Site is down? Ignore updates for now */
  if (NeedsUpdate.getStatus() != sf::Http::Response::Status::Ok)
    return true;

  std::string Body = NeedsUpdate.getBody();
  if (strncmp(Body.c_str(), "MPN-OK", 6))
    return true;
  else
  {
    std::stringstream BodyStream;
    BodyStream << Body;

    MsgAlert(false, MsgType::Information, "Dolphin-MPN is downloading a required update.");
    while (!BodyStream.eof())
    {
      mpn_download_t NewDownload;
      std::getline(BodyStream, NewDownload.LocalFile);
      std::getline(BodyStream, NewDownload.RemoteFile);
      m_Downloads.push_back(NewDownload);
    }
    downloadAll();
    exit(0);
    
    return false;
  }
}

bool MpnDownloadQueue::downloadAll()
{
  for (uint16_t i = 0; i < m_Downloads.size(); i++)
  {
    m_Http.setHost(MPN_WEBSITE);

    char LocalPathCmd[1024];
    snprintf(LocalPathCmd, sizeof(LocalPathCmd), "mkdir %s", m_Downloads[i].LocalPath.c_str());
    system(LocalPathCmd);
    sf::Http::Request Request(m_Downloads[i].RemoteFile);
    sf::Http::Response Response = m_Http.sendRequest(Request);
    std::ofstream File(m_Downloads[i].LocalFile);
    File.write(Response.getBody().c_str(), Response.getBody().size());
    File.close();
  }
  m_Downloads.clear();

  return true;
}

bool MpnDownloadQueue::pushBack(const std::string& PostData)
{
  sf::Http::Request Request("/getDownloads.php", sf::Http::Request::Post, "r=" + PostData);
  m_Http.setHost(MPN_WEBSITE);

  sf::Http::Response RequiredDownloads = m_Http.sendRequest(Request);
  if (RequiredDownloads.getStatus() != sf::Http::Response::Status::Ok)
    return false;

  std::string Body = RequiredDownloads.getBody();
  if (Body[0] == '\0')
    return false;

  std::stringstream BodyStream;
  BodyStream << Body;

  while (!BodyStream.eof())
  {
    mpn_download_t NewDownload;
    std::getline(BodyStream, NewDownload.LocalFile);
    std::getline(BodyStream, NewDownload.RemoteFile);
    m_Downloads.push_back(NewDownload);
  }

  return downloadAll();
}

#endif
