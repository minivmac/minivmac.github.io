#include "head.h"<br>
/*Encryption Code Copyright Orion Sky Lawlor, 1996*/<br>
/*"http://lawlor.uafdorms.alaska.edu/orion/programming.html"        */<br>
/*"ul" is "unsigned long" (4 bytes) */<br>
<br>
<br>
ul randS ,mulBy ,addBy ;<br>
ul randS2,mulBy2,addBy2;<br>
ul pos;<br>
<br>
/*Call initCoding with the password as a C string once before you begin*/<br>
/*encoding or decoding (it doesn't matter).*/<br>
void initCoding(char *pass)<br>
{<br>
//initialize the parameters.<br>
	static Boolean firstTime=true;<br>
	if (firstTime)<br>
	{<br>
		randS2=TickCount();<br>
		mulBy =184038653L;addBy =103701121L;//start with good, arbitrary numbers<br>
		mulBy2=194625377L;addBy2=203465495L;<br>
		short i;//mess with the password<br>
		for (i=0;pass[i];i++)<br>
			{}<br>
		short len=i;<br>
		if (len lessThan 2) //in my HTML translation, "less than" is ".lt."<br>
			error(131);<br>
		for (;i lessThan 41;i++)<br>
			pass[i]=pass[i%len]-i;<br>
		for (i=0;i lessThan 8;i++)<br>
		{//pervert these numbers by the password in a deterministic way.<br>
		//Recall that the low four bits of the password are the most random ones.<br>
		//hence, we will xor by the low two bits each iteration (masking the passwd)<br>
			mulBy^=(long(pass[i<<2])<<(i<<1))+pass[i+3];<br>
			addBy^=(long(pass[(i<<2)+1])<<(i<<1))+pass[i+1];<br>
			mulBy2^=(long(pass[(i<<2)+2])<<(i<<1))+pass[i+2];<br>
			addBy2^=(long(pass[(i<<2)+3])<<(i<<1))+pass[i+0];<br>
		}<br>
		firstTime=false;<br>
	}<br>
}<br>
<br>
/*Chop the message up any way you feel like, and pass each peice to xorCode.*/<br>
/*The business with the crcCheck is to ensure data integrity (and the right*/<br>
/*password).*/<br>
/*The way it works: the output of two separate random number generators*/<br>
/*are both xor'd to the data.  Since the random number generators (rand's)*/<br>
/*produce the same output when initialized in the same way, the result of*/<br>
/*doing this twice is the original data.  The intermediary data, however*/<br>
/*is well-hidden.  Without knowing the password, it's difficult to guess*/<br>
/*at the four thirty-two bit parameters to the random number generators.*/<br>
/*It's also pretty darn fast: on my '030 (386), it can process 250K per */<br>
/*second, including I/O time! */<br>
void xorCode(ul *dat,ul *crcCheck,ul numLongs,Boolean encode)<br>
{<br>
	if (encode)<br>
	{<br>
		for (long i=0;i lessThan numLongs;i++)<br>
		{<br>
			(*crcCheck)^=dat[i];<br>
			dat[i]^=(randS =randS *mulBy +addBy )<br>
				   ^(randS2=randS2*mulBy2+addBy2);<br>
		}<br>
	} else {<br>
		for (long i=0;i lessThan numLongs;i++)<br>
		{<br>
			dat[i]^=(randS =randS *mulBy +addBy )<br>
				   ^(randS2=randS2*mulBy2+addBy2);<br>
			(*crcCheck)^=dat[i];<br>
		}<br>
	}<br>
}