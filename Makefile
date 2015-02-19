resolver:	main.o Message.o Header.o ResourceRecord.o Question.o DNSConstants.o DNSCompression.o Encoder.o Base64Encoder.o EncodingFactory.o StringUtils.o UDPSocket.o UDP.o
			g++ -std=c++11 -I. -Wall *.o -o resolver

main.o:	main.cpp
		g++ -std=c++11 -I. -Wall -c main.cpp

Message.o:	Message.h Message.cpp
			g++ -std=c++11 -I. -Wall -c Message.cpp

Header.o:	Header.h Header.cpp
			g++ -std=c++11 -I. -Wall -c Header.cpp

ResourceRecord.o:	ResourceRecord.h ResourceRecord.cpp
					g++ -std=c++11 -I. -Wall -c ResourceRecord.cpp

Question.o:	Question.h Question.cpp
			g++ -std=c++11 -I. -Wall -c Question.cpp

DNSConstants.o:	DNSConstants.h
				g++ -std=c++11 -I. -Wall -c DNSConstants.h

DNSCompression.o:	DNSCompression.h DNSCompression.cpp
					g++ -std=c++11 -I. -Wall -c DNSCompression.cpp

Encoder.o:	Encoder.h
			g++ -std=c++11 -I. -Wall -c Encoder.h

Base64Encoder.o:	Base64Encoder.h Base64Encoder.cpp
					g++ -std=c++11 -I. -Wall -c Base64Encoder.cpp

EncodingFactory.o:	EncodingFactory.h EncodingFactory.cpp
					g++ -std=c++11 -I. -Wall -c EncodingFactory.cpp

StringUtils.o:	StringUtils.h StringUtils.cpp
				g++ -std=c++11 -I. -Wall -c StringUtils.cpp

UDPSocket.o:	UDPSocket.h UDPSocket.cpp
				g++ -std=c++11 -I. -Wall -c UDPSocket.cpp
UDP.o: 	UDP.h UDP.cpp
	g++ -std=c++11 -I. -Wall -c UDP.cpp

clean: 
	rm -f *.o
	rm -f resolver
