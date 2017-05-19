Cloud Computing - Container
******************************

.. contents::
    :backlinks: none
    :local:

Environment
-------------

================= ==============
Operating System  Ubuntu 16.04
docker version    1.12.6
runc version      1.0.0-rc2-dev
================= ==============

Structure
----------

Figure 1 shows the structure of this project, there are four directory

    * server
    * client
    * common
    * program

Server and Client directory contain the ``config.json``, ``rootfs``
which are the necessary info for ``runc``, ``run.sh`` is the script
to run the specific container, using ``sh run.sh`` to run. 



.. image:: 1.png

Figure 1.