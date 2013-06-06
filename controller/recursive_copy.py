#!/usr/bin/env python

# Designed to allow a recursive copy from the localhost to the remote
# host in Paramiko SFTP.  

from __future__ import print_function # for testing only
from __future__ import with_statement
from constants import *

# For testing only, paramiko:
import paramiko
import stat
import tempfile
import shutil

# Wrapper to allow use of with and good tear-down practice (adapted
# from http://effbot.org/zone/python-with-statement.htm).
class ParamikoTransportWrapper:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.transport = None

    def __enter__(self):
        self.transport = paramiko.Transport((self.host, self.port))
        return self.transport

    def __exit__(self, exc_type, exc_value, traceback):
        if self.transport is not None:
            self.transport.close()
        return False # allows any exception to be raised

# Wrapper to allow use of with and good tear-down practice (adapted
# from http://effbot.org/zone/python-with-statement.htm).
class ParamikoSFTPWrapper:
    def __init__(self, transport):
        self.transport = transport
        self.sftp = None

    def __enter__(self):
        self.sftp = paramiko.SFTPClient.from_transport(self.transport)
        return self.sftp

    def __exit__(self, exc_type, exc_value, traceback):
        if self.sftp is not None:
            self.sftp.close()
        return False # allows any exception to be raised


# recursive_walk adapted from the response by Jerub to
# http://stackoverflow.com/questions/120656/directory-listing-in-python
import os

# Recursively walks top, performing file_operation on each file and
# dir_operation on each directory.  Performs dir_operation on a
# directory just before performing file_operation on each file in that
# directory.  First call to dir_operation is on top.  Subdirectories
# are called in an order defined by os.walk.
#
# For details of onerror and followlinks (and more details on the
# recursive walk), see os.walk.
def recursive_walk(top, file_operation, dir_operation, onerror=None, followlinks=False):
    for dirpath, dirnames, filenames in os.walk(top, True, onerror, followlinks):
        # Perform dir_operation on the directory itself first.
        dir_operation(dirpath)

        # Perform file_operation on each file.
        for filename in filenames:
            file_operation(os.path.join(dirpath, filename))

        # for subdirname in dirnames:
        #     dir_operation(os.path.join(dirpath, subdirname))

        # # Advanced usage:
        # # editing the 'dirnames' list will stop os.walk() from recursing into there.
        # if '.git' in dirnames:
        #     # don't go into any .git directories.
        #     dirnames.remove('.git')


# For testing only, SFTP a directory using paramiko:
def sftp_testing(controller_top):
    host = "ec2-54-242-199-187.compute-1.amazonaws.com"
    port = 22
    username = "ec2-user"
    sshkeyfile = "/home/c/cs221x/dora/ssh-keys/steve-kuba-pair.pem"
    mykey = paramiko.RSAKey.from_private_key_file(sshkeyfile)

    # Note: using abspath and then basename should ensure that we get
    # just the directory's name (and no more of the path) out.  This
    # works whether controller_top is something like 'foo' or 'foo/'.
    controller_path = os.path.abspath(controller_top)
    web_server_path = '.'
	
    with ParamikoTransportWrapper(host,port) as transport:
        transport.use_compression()
        transport.connect(username = username, pkey = mykey)
        with ParamikoSFTPWrapper(transport) as sftp:
            # Note: will want to use functions not just lambdas to add
            # handy stuff like catching and ignoring IOErrors.. which
            # is perhaps what we want to do.  (Maybe log rather than
            # ignore!)
            recursive_walk(controller_path, 
                           #lambda filename: print("sftp.put", filename, os.path.join(web_server_path, os.path.relpath(filename, os.path.dirname(controller_path)))),
                           #lambda dirname: print("sftp.mkdir", os.path.join(web_server_path, os.path.relpath(dirname, os.path.dirname(controller_path)))))
                           #lambda filename: sftp.put(filename, os.path.join(web_server_path, os.path.relpath(filename, os.path.dirname(controller_path)))),
                           #lambda dirname: sftp.mkdir(os.path.join(web_server_path, os.path.relpath(dirname, os.path.dirname(controller_path)))))
                           lambda filename: print("sftp.put", filename, os.path.join(web_server_path, os.path.relpath(filename, os.path.dirname(controller_path))), sftp.put(filename, os.path.join(web_server_path, os.path.relpath(filename, os.path.dirname(controller_path))))),
                           lambda dirname: print("sftp.mkdir", os.path.join(web_server_path, os.path.relpath(dirname, os.path.dirname(controller_path))), sftp.mkdir(os.path.join(web_server_path, os.path.relpath(dirname, os.path.dirname(controller_path))))))
            print("Recursive delete on: %s" % os.path.join(web_server_path, os.path.basename(controller_path)))
            recursive_walk_remote(sftp, os.path.join(web_server_path, os.path.basename(controller_path)),
                                  lambda filename: print("sftp.remove", filename, sftp.remove(filename)),
                                  lambda dirname: print("sftp.rmdir", dirname, sftp.rmdir(dirname)))
    # sftp.close()
    # transport.close()

# Recursively walks top, performing file_operation on each file and
# dir_operation on each directory.  Performs dir_operation on a
# directory just before performing file_operation on each file in that
# directory.  First call to dir_operation is on top.  Subdirectories
# are called in an order defined by os.walk.
#
# For details of onerror and followlinks (and more details on the
# recursive walk), see os.walk.
def recursive_walk_remote(sftp, top, file_operation, dir_operation, topdown=False, onerror=None, followlinks=False):
    try:
        sftpattrib = None
        if followlinks:
            sftpattrib = sftp.stat(top)
        else:
            sftpattrib = sftp.lstat(top)

        if not hasattr(sftpattrib, 'st_mode'):
            raise "Unable to proceed with recursive_walk_remote b/c SFTP does not return the st_mode field used to tell whether items are files or directories on %s" % top

        if stat.S_ISDIR(sftpattrib.st_mode):
            # It's a directory.  Depending on pre vs. post order
            # selection, either operate on it first or recursively
            # operate on its children first.

            if topdown:
                dir_operation(top)
            
            # Question: could there be a problem if by this time top
            # is NO LONGER a directory?  Security hole?  Exception
            # issue?  Or, will exceptions on the far side handle this
            # correctly?
            for f in sftp.listdir(top):
                pathname = os.path.join(top, f)
                recursive_walk_remote(sftp, pathname, file_operation, dir_operation, topdown, onerror, followlinks)

            if not topdown:
                dir_operation(top)
        elif stat.S_ISREG(sftpattrib.st_mode) or stat.S_ISLNK(sftpattrib.st_mode):
            # It's a file (or a link).  Operate on it.
            file_operation(top)
        else:
            # Unknown file type.  What to do?
            print('Skipping %s' % top)
    except IOError as e:
        if onerror is None:
            pass
        else:
            onerror(e)


def recursive_walk_test_with_file_structure(locn = None):
    class TemporaryDirectory:
        def __init__(self, suffix, prefix, dir):
            self.suffix = suffix
            self.prefix = prefix
            self.dir = dir
            self.tempdir = None

        def __enter__(self):
            self.tempdir = tempfile.mkdtemp(self.suffix, self.prefix, self.dir)
            return self.tempdir

        def __exit__(self, exc_type, exc_value, traceback):
            if self.tempdir is not None:
                # Not too painful if the Temporary Directory stays around.
                # Just do due diligence to try to destroy it.
                shutil.rmtree(self.tempdir, True)
            # Allow any errors to propagate.
            return False

    # Create a temporary directory:
    with TemporaryDirectory('rec_walk_test', 'tmp', locn) as tempdir:
        # and fill it with:
        # Some files
        with open(os.path.join(tempdir, 'test1'), 'w') as f:
            f.write('test1\n')
        with open(os.path.join(tempdir, 'test2'), 'w') as f:
            f.write('test2\n')

        # One empty subdirectory;
        os.makedirs(os.path.join(tempdir, 'emptydir'))

        # One subdirectory with some files and a subdirectory (with a file) inside of it:
        os.makedirs(os.path.join(tempdir, 'bigdir', 'subdir'))
        with open(os.path.join(tempdir, 'bigdir', 'test3'), 'w') as f:
            f.write('test3\n')
        with open(os.path.join(tempdir, 'bigdir', 'test4'), 'w') as f:
            f.write('test4\n')
        with open(os.path.join(tempdir, 'bigdir', 'test5'), 'w') as f:
            f.write('test5\n')
        with open(os.path.join(tempdir, 'bigdir', 'subdir', 'test6'), 'w') as f:
            f.write('test6\n')
        
        # A link to the file in the sub-sub-directory and a link to the sub-sub-directory (for followlink testing).
        os.symlink(os.path.join(tempdir, 'bigdir', 'subdir', 'test6'),
                   os.path.join(tempdir, 'link1'))
        os.symlink(os.path.join(tempdir, 'bigdir', 'subdir'),
                   os.path.join(tempdir, 'link2'))

        # Note: CANNOT get unreadable file to actually be unreadable :(
        # # A file with no user read access (for error testing).
        # with open(os.path.join(tempdir, 'unreadable'), 'w') as f:
        #     f.write('unreadable\n')
        # try:
        #     os.chmod(os.path.join(tempdir, 'unreadable'), 0)
        #     results = []
        #     recursive_walk(tempdir, 
        #                    lambda f: results.append("File: %s" % f), 
        #                    lambda d: results.append("Dir: %s" % d), 
        #                    lambda e: results.append("Error: %s" % e.filename),
        #                    False)
        #     print(results)
        #     results = []
        #     recursive_walk(tempdir, 
        #                    lambda f: results.append("File: %s" % f), 
        #                    lambda d: results.append("Dir: %s" % d), 
        #                    lambda e: results.append("Error: %s" % e.filename),
        #                    True)
        #     print(results)
        # finally:
        #     os.chmod(os.path.join(tempdir, 'unreadable'), stat.S_IRUSR | stat.S_IWUSR)

        # TODO: automated testing/checking code.
        results = []
        recursive_walk(tempdir, 
                       lambda f: results.append("File: %s" % f), 
                       lambda d: results.append("Dir: %s" % d), 
                       lambda e: results.append("Error: %s" % e.filename),
                       False)
        print(results)
        results = []
        recursive_walk(tempdir, 
                       lambda f: results.append("File: %s" % f), 
                       lambda d: results.append("Dir: %s" % d), 
                       lambda e: results.append("Error: %s" % e.filename),
                       True)
        print(results)

        with ParamikoTransportWrapper('deas.ugrad.cs.ubc.ca', 22) as transport:
            transport.use_compression()
            transport.connect(username='cs221x', 
                              pkey=paramiko.DSSKey.from_private_key_file('/home/c/cs221x/.ssh/id_dsa_localhost'))
            with ParamikoSFTPWrapper(transport) as sftp:
                results = []
                recursive_walk_remote(sftp, tempdir,
                                      lambda f: results.append("File: %s" % f),
                                      lambda d: results.append("Dir: %s" % d))
                print(results)


    # Maybe the same for testing FTP but open an FTP connection to self??
