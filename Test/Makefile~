all:	test MakeTransparentImage

test: test.cpp
	g++ -o2 -std=c++14 test.cpp -o test -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

MakeTransparentImage: MakeTransparentImage.cpp
	g++ -o2 -std=c++14 MakeTransparentImage.cpp -o MakeTransparentImage -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal
