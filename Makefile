CXXFLAGS = -std=c++17 -Wall -Werror -O3 -g

RASTERIZER_OBJECTS = Scene.o Matrix4.o Helpers.o Main.o Vec3.o Vec4.o Color.o tinyxml2.o Rotation.o Translation.o Scaling.o Camera.o Mesh.o Triangle.o MiscTools.o

rasterizer: $(RASTERIZER_OBJECTS)
	$(CXX) $(CFLAGS) $(RASTERIZER_OBJECTS) -o rasterizer
	

Camera.o: Camera.cpp Camera.h

%.o: %.cpp %.h




hw-zip:
	zip e2457703_e2110252_rasterizer.zip *.cpp *.h

#png:
#	for foo in *.ppm; do convert "$$foo" "$${foo%.ppm}.png"; done


.PHONY: clean
clean:
	-rm rasterizer
	-rm *.o
	

