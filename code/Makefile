a.out:fibonacci array stl user_input*
	./fibonacci user_input*
	./array user_input*
	./stl user_input*

fibonacci:Fibonnaci_heap.cpp
	g++ Fibonnaci_heap.cpp -o fibonacci

array:Normal_array.cpp
	g++ Normal_array.cpp -o array

stl:STL_heap.cpp
	g++ STL_heap.cpp -o stl

user_input*:Generate_graph.cpp
	g++ Generate_graph.cpp -o graph
	./graph $(nodes) $(density) $(source)

clean:
	rm *.txt fibonacci array stl graph
