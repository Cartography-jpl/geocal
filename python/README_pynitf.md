PYNITF
======

pynitf is a python module that we have developed at JPL. This was code that
was originally part of geocal, but has been spun off as its own independent
repository. This both reduces the dependencies for using pynitf, and gives a
cleaner interface. Code is at https://github.jpl.nasa.gov/Cartography/pynitf.

Long term, we will probably want to treat this like any other python package
dependency, the fact that we wrote it doesn't really mean that this needs to
be handled differently (although we won't put this out on the standard python
package web site since it is not open source).

However, in the short term the library is still under active development. To
keep things in sync, we will maintain a copy of this code here in geocal,
and install it as part of the geocal install.

GIT Subtree
============

We us git subtree for managing the connection.

Note that git subtree isn't installed by default with git, however it is a
simple install from the git source tree, found in the contrib directory.

If the git repository is added as the remote pynitf-repo, then we pull
in the latest changes by:

    git subtree pull --prefix python/pynitf pynitf-repo master
(note we can include "--squash" here if we want to put the whole history
in as one pull).

Changes can be pushed up stream by:

    git subtree push --prefix python/pynitf pynitf-repo master

We can look at differences with upstream:

    git fetch pynitf-repo
    git diff HEAD:python/pynitf pynitf-repo/master

Note if you need to add in a new subtree, the initial creation is by:

    git subtree add --prefix python/pynitf pynitf-repo master

Sometimes you will get errors saying "Updates were rejected because the
tip of current branch is behind." First do a subtree pull if you haven't,
this may be real. But you might get this even if a pull says everything
is up to date. This appears to be a bug in subtree, see description at
http://stackoverflow.com/questions/13756055/git-subtree-subtree-up-to-date-but-cant-push

Can try 

    git push pynitf-repo `git subtree split --prefix python/pynitf master`:master --force
