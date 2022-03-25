all:
	rm -f infinitylights
	g++ -lusb-1.0  main.cpp InfinityPortal.cpp -o infinitylights

lightshow:
	rm -f lightshow
	g++ -lusb-1.0  lightshow.cpp InfinityPortal.cpp -o lightshow

clean:
	rm -f infinitylights lightshow