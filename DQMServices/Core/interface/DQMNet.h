#ifndef DQMSERVICES_CORE_DQM_NET_H
# define DQMSERVICES_CORE_DQM_NET_H

# include "classlib/iobase/InetServerSocket.h"
# include "classlib/iobase/IOSelector.h"
# include "classlib/iobase/Pipe.h"
# include "classlib/utils/Signal.h"
# include "classlib/utils/Error.h"
# include "classlib/utils/Time.h"
# include <pthread.h>
# include <stdint.h>
# include <iostream>
# include <vector>
# include <string>
# include <list>
# include <map>
# include <set>

//class DQMStore;

class DQMNet
{
public:
  static const uint32_t DQM_PROP_TYPE_MASK	= 0x000000ff;
  static const uint32_t DQM_PROP_TYPE_SCALAR	= 0x0000000f;
  static const uint32_t DQM_PROP_TYPE_INVALID	= 0x00000000;
  static const uint32_t DQM_PROP_TYPE_INT	= 0x00000001;
  static const uint32_t DQM_PROP_TYPE_REAL	= 0x00000002;
  static const uint32_t DQM_PROP_TYPE_STRING	= 0x00000003;
  static const uint32_t DQM_PROP_TYPE_TH1F	= 0x00000010;
  static const uint32_t DQM_PROP_TYPE_TH1S	= 0x00000011;
  static const uint32_t DQM_PROP_TYPE_TH1D	= 0x00000012;
  static const uint32_t DQM_PROP_TYPE_TH2F	= 0x00000020;
  static const uint32_t DQM_PROP_TYPE_TH2S	= 0x00000021;
  static const uint32_t DQM_PROP_TYPE_TH2D	= 0x00000022;
  static const uint32_t DQM_PROP_TYPE_TH3F	= 0x00000030;
  static const uint32_t DQM_PROP_TYPE_TH3S	= 0x00000031;
  static const uint32_t DQM_PROP_TYPE_TH3D	= 0x00000032;
  static const uint32_t DQM_PROP_TYPE_TPROF	= 0x00000040;
  static const uint32_t DQM_PROP_TYPE_TPROF2D	= 0x00000041;
  
  static const uint32_t DQM_PROP_REPORT_MASK	= 0x00000f00;
  static const uint32_t DQM_PROP_REPORT_CLEAR	= 0x00000000;
  static const uint32_t DQM_PROP_REPORT_ERROR	= 0x00000100;
  static const uint32_t DQM_PROP_REPORT_WARN	= 0x00000200;
  static const uint32_t DQM_PROP_REPORT_OTHER	= 0x00000400;
  static const uint32_t DQM_PROP_REPORT_ALARM	= (DQM_PROP_REPORT_ERROR
						   | DQM_PROP_REPORT_WARN
						   | DQM_PROP_REPORT_OTHER);

  static const uint32_t DQM_PROP_HAS_REFERENCE	= 0x00001000;
  static const uint32_t DQM_PROP_TAGGED		= 0x00002000;
  static const uint32_t DQM_PROP_ACCUMULATE	= 0x00004000;
  static const uint32_t DQM_PROP_RESET		= 0x00008000;

  static const uint32_t DQM_PROP_NEW		= 0x00010000;
  static const uint32_t DQM_PROP_RECEIVED	= 0x00020000;
  static const uint32_t DQM_PROP_INTERESTING	= 0x00040000;
  static const uint32_t DQM_PROP_DEAD		= 0x00080000;

  static const uint32_t DQM_MSG_HELLO		= 0;
  static const uint32_t DQM_MSG_UPDATE_ME	= 1;
  static const uint32_t DQM_MSG_LIST_OBJECTS	= 2;
  static const uint32_t DQM_MSG_GET_OBJECT	= 3;

  static const uint32_t DQM_REPLY_LIST_BEGIN	= 101;
  static const uint32_t DQM_REPLY_LIST_END	= 102;
  static const uint32_t DQM_REPLY_NONE		= 103;
  static const uint32_t DQM_REPLY_OBJECT	= 104;

  static const uint32_t MAX_PEER_WAITREQS	= 128;

  struct Peer;
  struct QValue;
  struct WaitObject;

  typedef std::vector<unsigned char>    DataBlob;
  typedef std::vector<QValue>		QReports;
  typedef std::vector<uint32_t>		TagList; // DEPRECATED
  typedef std::list<WaitObject>		WaitList;

  struct QValue
  {
    int			code;
    float		qtresult;
    std::string		message;
    std::string		qtname;
    std::string		algorithm;
  };

  struct CoreObject
  {
    uint32_t		flags;
    uint32_t 		tag;
    uint64_t		version;
    const std::string	*dirname;
    std::string		objname;
    QReports		qreports;
  };
  
  struct Object : CoreObject
  {
    DataBlob		rawdata;
    std::string		scalar;
    std::string		qdata;
  };

  struct Bucket
  {
    Bucket		*next;
    DataBlob		data;
  };

  struct WaitObject
  {
    lat::Time		time;
    std::string		name;
    std::string		info;
    Peer		*peer;
  };

  struct AutoPeer;
  struct Peer
  {
    std::string		peeraddr;
    lat::Socket		*socket;
    DataBlob		incoming;
    Bucket		*sendq;
    size_t		sendpos;

    unsigned		mask;
    bool		source;
    bool		update;
    bool		updated;
    size_t		updates;
    size_t		waiting;
    AutoPeer		*automatic;
  };

  struct AutoPeer
  {
    Peer		*peer;
    lat::Time		next;
    std::string		host;
    int			port;
    bool		update;
  };

  DQMNet(const std::string &appname = "");
  virtual ~DQMNet(void);

  void			debug(bool doit);
  void			delay(int delay);
  void			startLocalServer(int port);
  void			updateToCollector(const std::string &host, int port);
  void			listenToCollector(const std::string &host, int port);
  void			shutdown(void);
  void			lock(void);
  void			unlock(void);

  void			start(void);
  void			run(void);

  virtual void		updateLocalObject(Object &o);
  virtual bool		removeLocalExcept(const std::set<std::string> &known);
  void			sendLocalChanges(void);

  typedef bool (*SetOrder)(const CoreObject &, const CoreObject &);
  static bool setOrder(const CoreObject &a, const CoreObject &b)
    {
      int diff = a.dirname->compare(*b.dirname);
      return (diff < 0 ? true
	      : diff == 0 ? a.objname < b.objname
	      : false);
    }

  static void		packQualityData(std::string &into, const QReports &qr);
  static void		unpackQualityData(QReports &qr, uint32_t &flags, const char *from);

protected:
  std::ostream &	logme(void);
  static void		copydata(Bucket *b, const void *data, size_t len);
  virtual void		sendObjectToPeer(Bucket *msg, Object &o, bool data);

  virtual bool		shouldStop(void);
  void			waitForData(Peer *p, const std::string &name, const std::string &info, Peer *owner);
  virtual void		releaseFromWait(Bucket *msg, WaitObject &w, Object *o);
  virtual bool		onMessage(Bucket *msg, Peer *p, unsigned char *data, size_t len);

  // bool			reconstructObject(Object &o);
  // bool			reinstateObject(DQMStore *store, Object &o);
  virtual Object *	findObject(Peer *p, const std::string &name, Peer **owner = 0) = 0;
  virtual Object *	makeObject(Peer *p, const std::string &name) = 0;
  virtual void		markObjectsDead(Peer *p) = 0;
  virtual void		purgeDeadObjects(Peer *p) = 0;

  virtual Peer *	getPeer(lat::Socket *s) = 0;
  virtual Peer *	createPeer(lat::Socket *s) = 0;
  virtual void		removePeer(Peer *p, lat::Socket *s) = 0;
  virtual void		sendObjectListToPeer(Bucket *msg, bool all, bool clear) = 0;
  virtual void		sendObjectListToPeers(bool all) = 0;

  void			updateMask(Peer *p);
  virtual void		updatePeerMasks(void) = 0;
  static void		discard(Bucket *&b);

  bool			debug_;
  pthread_mutex_t	lock_;

private:
  void			losePeer(const char *reason,
				 Peer *peer,
				 lat::IOSelectEvent *event,
				 lat::Error *err = 0);
  void			requestObject(Peer *p, const char *name, size_t len);
  void			releaseFromWait(WaitList::iterator i, Object *o);
  void			releaseWaiters(const std::string &name, Object *o);

  bool			onPeerData(lat::IOSelectEvent *ev, Peer *p);
  bool			onPeerConnect(lat::IOSelectEvent *ev);
  bool			onLocalNotify(lat::IOSelectEvent *ev);

  std::string		appname_;
  int			pid_;

  lat::IOSelector	sel_;
  lat::InetServerSocket	*server_;
  lat::Pipe		wakeup_;
  lat::Time		version_;

  AutoPeer		upstream_;
  AutoPeer		downstream_;
  WaitList		waiting_;

  pthread_t		communicate_;
  sig_atomic_t		shutdown_;

  int			delay_;
  bool			flush_;

  // copying is not available
  DQMNet(const DQMNet &);
  DQMNet &operator=(const DQMNet &);
};

template <class ObjType>
class DQMImplNet : public DQMNet
{
public:
  struct ImplPeer;

  typedef std::set<std::string> DirMap;
  typedef std::set<ObjType, SetOrder> ObjectMap;
  typedef std::map<lat::Socket *, ImplPeer> PeerMap;
  struct ImplPeer : Peer
  {
    ImplPeer(void) : objs(setOrder) {}
    ObjectMap objs;
    DirMap dirs;
  };

  DQMImplNet(const std::string &appname = "")
    : DQMNet(appname)
    {}
  
  ~DQMImplNet(void)
    {}

protected:
  virtual Object *
  findObject(Peer *p, const std::string &name, Peer **owner = 0)
    {
      size_t slash = name.rfind('/');
      size_t dirpos = (slash == std::string::npos ? 0 : slash);
      size_t namepos = (slash == std::string::npos ? 0 : slash+1);
      std::string path(name, 0, dirpos);
      ObjType proto;
      proto.dirname = &path;
      proto.objname.append(name, namepos, std::string::npos);

      typename ObjectMap::iterator pos;
      typename PeerMap::iterator i, e;
      if (owner)
	*owner = 0;
      if (p)
      {
	ImplPeer *ip = static_cast<ImplPeer *>(p);
	pos = ip->objs.find(proto);
	if (pos == ip->objs.end())
	  return 0;
	else
	{
	  if (owner) *owner = ip;
	  return const_cast<ObjType *>(&*pos);
	}
      }
      else
      {
	for (i = peers_.begin(), e = peers_.end(); i != e; ++i)
	{
	  pos = i->second.objs.find(proto);
	  if (pos != i->second.objs.end())
	  {
	    if (owner) *owner = &i->second;
	    return const_cast<ObjType *>(&*pos);
	  }
	}
	return 0;
      }
    }

  virtual Object *
  makeObject(Peer *p, const std::string &name)
    {
      ImplPeer *ip = static_cast<ImplPeer *>(p);
      size_t slash = name.rfind('/');
      size_t dirpos = (slash == std::string::npos ? 0 : slash);
      size_t namepos = (slash == std::string::npos ? 0 : slash+1);
      ObjType o;
      o.flags = 0;
      o.tag = 0;
      o.version = 0;
      o.dirname = &*ip->dirs.insert(name.substr(0, dirpos)).first;
      o.objname.append(name, namepos, std::string::npos);
      return const_cast<ObjType *>(&*ip->objs.insert(o).first);
    }

  // Mark all the objects dead.  This is intended to be used when
  // starting to process a complete list of objects, in order to
  // flag the objects that need to be killed at the end.  After
  // call to this method, revive all live objects by removing the
  // DQM_PROP_DEAD flag, then call purgeDeadObjects() at the end
  // to remove the dead ones.
  virtual void
  markObjectsDead(Peer *p)
    {
      ImplPeer *ip = static_cast<ImplPeer *>(p);
      typename ObjectMap::iterator i, e;
      for (i = ip->objs.begin(), e = ip->objs.end(); i != e; ++i)
	const_cast<ObjType &>(*i).flags |= DQM_PROP_DEAD;
    }

  // Mark remaining zombie objects as dead.  See markObjectsDead().
  virtual void
  purgeDeadObjects(Peer *p)
    {
      ImplPeer *ip = static_cast<ImplPeer *>(p);
      typename ObjectMap::iterator i, e;
      for (i = ip->objs.begin(), e = ip->objs.end(); i != e; )
	if (i->flags & DQM_PROP_DEAD)
	  ip->objs.erase(i++);
	else
	  ++i;
    }

  virtual Peer *
  getPeer(lat::Socket *s)
    {
      typename PeerMap::iterator pos = peers_.find(s);
      typename PeerMap::iterator end = peers_.end();
      return pos == end ? 0 : &pos->second;
    }

  virtual Peer *
  createPeer(lat::Socket *s)
    {
      ImplPeer *ip = &peers_[s];
      ip->socket = 0;
      ip->sendq = 0;
      ip->sendpos = 0;
      ip->mask = 0;
      ip->source = false;
      ip->update = false;
      ip->updated = false;
      ip->updates = 0;
      ip->waiting = 0;
      ip->automatic = 0;
      return ip;
    }

  virtual void
  removePeer(Peer *p, lat::Socket *s)
    {
      ImplPeer *ip = static_cast<ImplPeer *>(p);
      bool needflush = ! ip->objs.empty();

      typename ObjectMap::iterator i, e;
      for (i = ip->objs.begin(), e = ip->objs.end(); i != e; )
	ip->objs.erase(i++);
    
      peers_.erase(s);

      // If we removed a peer with objects, our list of objects
      // has changed and we need to update downstream peers.
      if (needflush)
	sendLocalChanges();
    }

  /// Send all objects to a peer and optionally mark sent objects old.
  virtual void
  sendObjectListToPeer(Bucket *msg, bool all, bool clear)
    {
      typename PeerMap::iterator pi, pe;
      typename ObjectMap::iterator oi, oe;
      uint32_t numobjs = 0;
      for (pi = peers_.begin(), pe = peers_.end(); pi != pe; ++pi)
	numobjs += pi->second.objs.size();

      msg->data.reserve(msg->data.size() + 300*numobjs);

      uint32_t nupdates = 0;
      uint32_t words [4];
      words[0] = sizeof(words);
      words[1] = DQM_REPLY_LIST_BEGIN;
      words[2] = numobjs;
      words[3] = all;
      copydata(msg, &words[0], sizeof(words));

      for (pi = peers_.begin(), pe = peers_.end(); pi != pe; ++pi)
	for (oi = pi->second.objs.begin(), oe = pi->second.objs.end(); oi != oe; ++oi)
	  if (all || (oi->flags & DQM_PROP_NEW))
	  {
	    sendObjectToPeer(msg, const_cast<ObjType &>(*oi), false);
	    if (clear)
	      const_cast<ObjType &>(*oi).flags &= ~DQM_PROP_NEW;
	    ++nupdates;
	  }

      words[1] = DQM_REPLY_LIST_END;
      words[2] = nupdates;
      copydata(msg, &words[0], sizeof(words));
    }

  virtual void
  sendObjectListToPeers(bool all)
    {
      typename PeerMap::iterator i, e;
      for (i = peers_.begin(), e = peers_.end(); i != e; ++i)
      {
	ImplPeer &p = i->second;
	if (! p.update)
	  continue;

	if (debug_)
	  logme()
	    << "DEBUG: notifying " << p.peeraddr << std::endl;

	Bucket msg;
        msg.next = 0;
	sendObjectListToPeer(&msg, !p.updated || all, true);

	if (! msg.data.empty())
	{
	  Bucket **prev = &p.sendq;
	  while (*prev)
	    prev = &(*prev)->next;

	  *prev = new Bucket;
	  (*prev)->next = 0;
	  (*prev)->data.swap(msg.data);
	}
	p.updated = true;
      }
    }

  virtual void
  updatePeerMasks(void)
    {
      typename PeerMap::iterator i, e;
      for (i = peers_.begin(), e = peers_.end(); i != e; )
	updateMask(&(i++)->second);
    }

protected:
  PeerMap		peers_;
};
  

class DQMBasicNet : public DQMImplNet<DQMNet::Object>
{
public:
  DQMBasicNet(const std::string &appname = "");

protected:
  virtual void		updateLocalObject(Object &o);
  virtual bool		removeLocalExcept(const std::set<std::string> &known);

private:
  ImplPeer		*local_;
};


#endif // DQMSERVICES_CORE_DQM_NET_H
