# GPS2Cartesian
Using proj.4 libraries, convert longitude and latitude to cartesian coordiante , and inverse convert

1.Environment

ubuntu14.04 (x64)

2.Third party

2.1 install eigen3 in ubuntu

	sudo apt-get install libeigen3-dev
    
  or get it from http://eigen.tuxfamily.org/index.php?title=Main_Page
  
2.2 proj.4 install in ubuntu

	git clone https://github.com/OSGeo/proj.4.git
    
	cd proj.4
    
    mkdir build
    
	cd build
    
	cmake ..
    
	make
    
	sudo make install
