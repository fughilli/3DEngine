compiler = g++

renderer:
	$(compiler) --std=c++11 -Wno-c++11-narrowing -o renderer *.cpp 3DEngineRenderModule/*.cpp VectorLib/*.cpp res/headers/*.cpp
renderer-parallel:
	$(compiler) -fopenmp --std=c++11 -Wno-c++11-narrowing -o renderer *.cpp 3DEngineRenderModule/*.cpp VectorLib/*.cpp res/headers/*.cpp
clean:
	rm -f renderer
