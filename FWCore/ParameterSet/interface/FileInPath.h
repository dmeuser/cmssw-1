#ifndef ParameterSet_FileInPath_h
#define ParameterSet_FileInPath_h

/// $Id:$
///

/// Find a non-event-data file, given a relative path.

/// FileInPath knows how to take a string, interpreted as a relative
/// path to a file, and to interpret using the "standard CMS
/// non-event-data file searching mechanism".
///
/// The mechanism using the environment variables:
///    CMS_SEARCH_PATH: may be set by the end-user
///    CMSDATA:         should be set by a site administrator
///
///  CMS_SEARCH_PATH is a 'search path' limited to either 1 or 2
///  components. The legal values are:
///
///       ".", which means to search for files under the
///            process's current working directory. Changing
///            the current working directory while running a
///            program will confuse this!
///
///       "CMSDATA", which means search the "official place",
///                  defined by the value of the CMSDATA
///                  environment variable, for files.
///
///       ".:CMSDATA", which means look first in the current
///                    working directory, then in the "official
///                    place", for files.

// Notes:
//
//  1. We do not deal well with paths that contain spaces; this is because
//     of the way the ParameterSet system's 'encode' and 'decode' functions
//     are implemented for FileInPath objects. This could be fixed, if it
//     is important to handle filenames or paths with embedded spaces.


// TODO: Find the correct package for this class to reside. It
// doesn't seem well-suited for ParameterSet.


#include <istream>
#include <ostream>
#include <string>


namespace edm
{
  class FileInPath
  {
  public:

    /// Default c'tor does no file-existence check; what file would it
    /// check for existence?
    FileInPath();

    /// We throw an exception is the referenced file is not found.
    explicit FileInPath(const std::string& r);
    explicit FileInPath(const char* r);

    /// Return a string containing the canonical form of the
    /// *relative* path. DO NOT USE THIS AS THE FILENAME for any file
    /// operations; use filename() for that purpose.
    std::string const& relativePath() const;

    /// Was the file found under "."?
    bool isLocal() const;

    /// Return a string that can be used to open the referenced
    /// file. 
    ///
    /// Note that operations on this file may fail, including
    /// testing for existence. This is because the state of a
    /// filesystem is global; other threads, processes, etc., may have
    /// removed the file since we checked on its existence at the time
    /// of construction of the FileInPath object.
    std::string const& filename() const;

    /// Write contents to the given ostream.
    /// Writing errors are reflected in the state of the stream.
    void write(std::ostream& os) const;
    
    /// Read from the given istream, and set contents accordingly.
    /// Reading errors are reflected in the state of the stream.
    void read(std::istream& is);

  private:
    std::string    relativePath_;
    std::string    canonicalFilename_;
    bool           isLocal_;


    // Helper function for construction.
    void initialize_();
  };


  inline  std::ostream& 
  operator<< (std::ostream& os, const FileInPath& fip)
  {
    fip.write(os);
    return os;
  }

  inline std::istream&
  operator>> (std::istream& is, FileInPath& fip)
  {
    fip.read(is);
    return is;
  }

}

#endif
