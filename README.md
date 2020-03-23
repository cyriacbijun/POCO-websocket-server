# POCO-websocket-server
This repo contains the code for running a websocket server using the POCO C++ libraries. Two html files are also given to demonstrate.

### Setting up POCO

I assume you will be using a \*unix environment. I am going to show for Debian derived Distros. 
If you want to proceed on Windows, check out [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10).
I have used this to get a Debian terminal and it sure works!!
##### Prerequites
install cmake,openssl and other libraries( you'll know what to install when the error appears). Any cmake error->search stackoverflow, it is sure to solve it. Install like the following:

`sudo apt install cmake g++`

##### Download POCO and setup

```bash
cyriac@Cerebro:/home$wget https://pocoproject.org/releases/poco-1.10.1/poco-1.10.1-all.tar.gz
cyriac@Cerebro:/home$tar -xvf poco-1.10.1-all.tar.gz
```

That's gonna take some time.After it extracts, we create a build directory in the same folder. Then, change the directory to poco-1.10.1-all. Here, we set the build folder to the previous build folder and cmake checks the system.The final command builds the POCO Library according to our needs and copies into our build folder.
 
```bash
cyriac@Cerebro:/home$mkdir build
cyriac@Cerebro:/home$cd poco-1.10.1-all
cyriac@Cerebro:/home/poco-1.10.1-all$cmake . -DCMAKE_INSTALL_PREFIX=../build/
cyriac@Cerebro:/home/poco-1.10.1-all$cmake --build . --target install
```

Now,go back to `/home` and check the build folder to see the same output as mine. If done, you are all set.

```bash
cyriac@Cerebro:/home$cd ../build/
cyriac@Cerebro:/home/build$ls
bin  include  lib
```

##### Download the git files

```bash
cyriac@Cerebro:/home/build$ cd .. && git clone 
