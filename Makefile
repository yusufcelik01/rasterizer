CXXFLAGS = -std=c++17 -Wall -Werror -O3 -g

RASTERIZER_OBJECTS = Scene.o Matrix4.o Helpers.o Main.o Vec3.o Vec4.o Color.o tinyxml2.o Rotation.o Translation.o Scaling.o Camera.o Mesh.o Triangle.o MiscTools.o Line.o


rasterizer: $(RASTERIZER_OBJECTS)
	$(CXX) $(CFLAGS) $(RASTERIZER_OBJECTS) -o rasterizer
	


%.o: %.cpp %.h


png:
	for foo in *.ppm; do convert "$$foo" "$${foo%.ppm}.png"; done


.PHONY: clean
clean:
	-rm rasterizer
	-rm *.o
	
Culling_disabled:
	for foo in culling_disabled_inputs/*xml; do bar="time_$${foo#inputs/}"; echo "./rasterizer $$foo";  ./rasterizer "$$foo" ;  done

Culling_enabled:
	for foo in culling_enabled_inputs/*xml; do bar="time_$${foo#inputs/}"; echo "./rasterizer $$foo";  ./rasterizer "$$foo" ;  done

Clipping:
	for foo in clipping_example/*xml; do bar="time_$${foo#inputs/}"; echo "./rasterizer $$foo";  ./rasterizer "$$foo" ;  done
