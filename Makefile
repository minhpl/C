all:	sortvector sortlist sortint lambda a intersectsearch distancesearch drawimage DrawCircleVer1 DrawCircleVer2 DrawCircleVer3 DrawPolygon DrawPolygon2 DrawPolygon3 DrawPolygon4 DrawPolygon5 DrawPolygon6 DrawPolygon7

sortvector:	SortVector.cpp
	g++ -std=c++14 SortVector.cpp -o sortvector
	
sortlist: SortList.cpp
	g++ -02 -std=c++14 SortList.cpp -o sortlist
	
sortint: sortint.cpp
	g++ -02 -std=c++14 sortint.cpp -o sortint
	
lambda: lambda.cpp
	g++ -02 -std=c++14 lambda.cpp -o lambda

intersect: intersect.cpp
	g++ -02 -std=c++14 intersect.cpp -o intersect

distancesearch: distancesearch.cpp
	g++ -02 -std=c++14 distancesearch.cpp -o distancesearch

drawimage: drawimage.cpp
	g++ -02 -std=c++14 drawimage.cpp -o drawimage -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

drawimage: drawimage.cpp
	g++ -02 -std=c++14 drawimage.cpp -o drawimage -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawCircleVer1: DrawCircleVer1.cpp
	g++ -02 -std=c++14 DrawCircleVer1.cpp -o DrawCircleVer1 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawCircleVer2: DrawCircleVer2.cpp
	g++ -02 -std=c++14 DrawCircleVer2.cpp -o DrawCircleVer2 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawCircleVer3: DrawCircleVer3.cpp
	g++ -02 -std=c++14 DrawCircleVer3.cpp -o DrawCircleVer3 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawPolygon: DrawPolygon.cpp
	g++ -02 -std=c++14 DrawPolygon.cpp -o DrawPolygon -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawPolygon2: DrawPolygon2.cpp 
	g++ -02 -std=c++14 DrawPolygon2.cpp -o DrawPolygon2 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

MakePolygon: MakePolygon.cpp
	g++ -O2 -std=c++14 MakePolygon.cpp -o MakePolygon -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawPolygon3: DrawPolygon3.cpp MakePolygon.h
	g++ -O2 -std=c++14 DrawPolygon3.cpp -o DrawPolygon3 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawPolygon4: DrawPolygon4.cpp MakePolygon.h
	g++ -O2 -std=c++14 DrawPolygon4.cpp -o DrawPolygon4 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

DrawPolygon5: DrawPolygon5.cpp MakePolygon.h
	g++ -O2 -std=c++14 DrawPolygon5.cpp -o DrawPolygon5 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal \
	                -lpthread

DrawPolygon6: DrawPolygon6.cpp MakePolygon.h
	g++ -O2 -std=c++14 DrawPolygon6.cpp -o DrawPolygon6 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal \
	                -lpthread

#song song
DrawPolygon7: DrawPolygon7.cpp MakePolygon.h
	g++ -O2 -std=c++14 DrawPolygon7.cpp -o DrawPolygon7 -I/usr/include/opencv -L/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal \
	                -lpthread
