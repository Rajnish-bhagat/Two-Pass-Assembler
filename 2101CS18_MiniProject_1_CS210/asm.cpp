/*****************************************************************************
TITLE: Assembler																														
AUTHOR: Bhagat Rajnish Omprakash 
ROLL NUMBER: 2101CS18
Declaration of Authorship
This cpp file, asm.cpp, is part of the miniproject of CS209/CS210 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/

#include<bits/stdc++.h>
using namespace std;
 
int A,B,PC=0,SP,flag=0; // A & B as internal stack, PC is program counter and SP is stack pointer 

map<string,string>m,data,SET; //map "m" to store Instruction Set, "data" to store data values, "SET" to store SET values

map<string,int>m2,m3,m4; //maps to store m2-Labels,check duplicate in pass1,pass2,func


// Mnemonic Struct for Table_of_Inst
 
// operand_req = 0 : No Operand
// operand_req = 1 : Value Operand
// operand_req = 2 : 'data'
// operand_req = 3 : 'set'
// operand_req = 4 : Offset Operand

map<string,pair<string,int>>Table_of_Inst;



bool isOctal(string s)
{
// Funtion to check whether the string is Octal or not
    if((int) s.size() < 2)
    	return false;
    bool ok = true;
    for(int i = 1; i < (int)s.size(); i++){
        ok &= (s[i] >= '0' and s[i] <= '7');
    }
    return ok & (s[0] == '0');
}

bool isHexadecimal(string s)
{
// Funtion to check whether the string is in HexaDecimal form or not
    bool state = true ;
    if((int)s.size() < 3)
    	return false;
    state &= (s[0] == '0') & ((s[1] == 'x' or s[1] == 'X'));
    for(int i = 2; i < (int) s.size(); i++){
        bool st = (s[i] >= '0' and s[i] <= '9');
        st |= ((s[i] >= 'a' and s[i] <= 'f') or ((s[i] >= 'A' and s[i] <= 'F')));
        state &= st;
    }
    return state;
}

bool isDecimal(string s)
{
// Funtion to check whether the string is in decimal form or not
	bool ok = true;
  if(s[0]=='-' || s[0]=='+')
  {
    s=s.substr(1,s.size()-1);
  }
	for(int i = 0; i < (int)s.size(); i++)
		ok &= (s[i] >= '0' and s[i] <= '9');
	return ok;
}

bool isBinary(string s)
{
// Funtion to check whether the string is in decimal form or not
   for(int i=0;i<s.size();i++)
   {
       if(s[i]!='1' && s[i]!='0')
       return false;
   }
   return true;
}

string convertHexa(int number,int digits)
{
  // converts "number" to hexadecimal format of "digits" number of digits
    string hexa;
    int c=0;
    while (++c<=digits)
    {
        int rem = number % 16;
       
        if (rem < 10)
            hexa.push_back(rem + '0');
        else
            hexa.push_back(rem - 10 + 'A');

        number = number / 16;
    }
    reverse(hexa.begin(), hexa.end());
    return hexa;
}

string conv6DigitHex(string s)
{
  // changes length of hexadecimal string to 6 digits
    int x=s.size();
    x-=2;
    s=s.substr(2,x);
    int count=0;
    while(x+count!=6)
    {
        s="0"+s;
        count++;
    }
    return s;
}

string conv8DigitHex(string s)
{
  // changes length of hexadecimal string to 8 digits and capitalizes all small letters
    int x=s.size();
    x-=2;
    s=s.substr(2,x);
    int count=0;
    while(x+count!=8)
    {
        s="0"+s;
        count++;
    }
    for(int i=0;i<s.size();i++)
    {
      if(s[i]<='z' && s[i]>='a')
      {
         char z='A'+s[i]-'a';
         s[i]=z;
      }
    }
    return s;
}

string convNegHex6Dig(int n,int dig)
{
  // changes decimal to binary to Hexadecimal, also handles negative numbers by taking two's complement
    int c=0;
    int arr[dig]={0};
    while(c<dig)
    {
        arr[c]=n%2;
        n/=2;
        c++;
    }
    for(int i=0;i<dig;i++)
    {
        arr[i]+=1;
        arr[i]%=2;
    }
    c=1;
    
    for(int i=0;i<dig;i++)
    {
        if(arr[i]==0 && c==1)
        {
            arr[i]=1;c=0;
            break;
        }
        else
        {
            arr[i]=0;
        }
    }
    string ret;
    
    for(int i=dig-1;i>=0;i-=4)
    {
        if(arr[i]==1 && arr[i-1]==1 && arr[i-2]==1 && arr[i-3]==1)
        {
            ret+="F";
        }
        else if(arr[i]==1 && arr[i-1]==1 && arr[i-2]==1 && arr[i-3]==0)
        {
            ret+="E";
        }
        else if(arr[i]==1 && arr[i-1]==1 && arr[i-2]==0 && arr[i-3]==1)
        {
            ret+="D";
        }
        else if(arr[i]==1 && arr[i-1]==1 && arr[i-2]==0 && arr[i-3]==0)
        {
            ret+="C";
        }
        else if(arr[i]==1 && arr[i-1]==0 && arr[i-2]==1 && arr[i-3]==1)
        {
            ret+="B";
        }
        else if(arr[i]==1 && arr[i-1]==0 && arr[i-2]==1 && arr[i-3]==0)
        {
            ret+="A";
        }
        else if(arr[i]==1 && arr[i-1]==0 && arr[i-2]==0 && arr[i-3]==1)
        {
            ret+="9";
        }
        else if(arr[i]==1 && arr[i-1]==0 && arr[i-2]==0 && arr[i-3]==0)
        {
            ret+="8";
        }
        else if(arr[i]==0 && arr[i-1]==1 && arr[i-2]==1 && arr[i-3]==1)
        {
            ret+="7";
        }
        else if(arr[i]==0 && arr[i-1]==1 && arr[i-2]==1 && arr[i-3]==0)
        {
            ret+="6";
        }
        else if(arr[i]==0 && arr[i-1]==1 && arr[i-2]==0 && arr[i-3]==1)
        {
            ret+="5";
        }
        else if(arr[i]==0 && arr[i-1]==1 && arr[i-2]==0 && arr[i-3]==0)
        {
            ret+="4";
        }
        else if(arr[i]==0 && arr[i-1]==0 && arr[i-2]==1 && arr[i-3]==1)
        {
            ret+="3";
        }
        else if(arr[i]==0 && arr[i-1]==0 && arr[i-2]==1 && arr[i-3]==0)
        {
            ret+="2";
        }
        else if(arr[i]==0 && arr[i-1]==0 && arr[i-2]==0 && arr[i-3]==1)
        {
            ret+="1";
        }
        else if(arr[i]==0 && arr[i-1]==0 && arr[i-2]==0 && arr[i-3]==0)
        {
            ret+="0";
        }
    }
    
    return ret;
}

string HexToBin(string hexdec)
{
  //converts HExadecimal to Binary
    int i = 0;
    string bin;
 
    while (hexdec[i]) 
    {
        switch (hexdec[i]) 
        {
        case '0':
            bin+="0000";
            break;
        case '1':
            bin+="0001";
            break;
        case '2':
            bin+="0010";
            break;
        case '3':
            bin+="0011";
            break;
        case '4':
            bin+="0100";
            break;
        case '5':
            bin+="0101";
            break;
        case '6':
            bin+="0110";
            break;
        case '7':
            bin+="0111";
            break;
        case '8':
            bin+="1000";
            break;
        case '9':
            bin+="1001";
            break;
        case 'A':
            bin+="1010";
            break;
        case 'B':
            bin+="1011";
            break;
        case 'C':
            bin+="1100";
            break;
        case 'D':
            bin+="1101";
            break;
        case 'E':
            bin+="1110";
            break;
        case 'F':
            bin+="1111";
            break;
        
        }
        i++;
    }
    return bin;
}


void func()
{
  // prints the errors

   FILE *ip;
   
   ifstream infile("input.asm"); //To read the input file consisting of code in assembly language

   ofstream outfile("error.log"); //To print the errors in the code

   string s,org; //To read each line of input.asm file
   PC=0;
   while(getline(infile,s) && ++PC) //Incrementing the program counter each time
   {
     org=s;
     int l=s.size(),posc=-1,poss=-1,r=0; //posc is the variable to store position of colon in the input string and poss stores position of semicolon in the input string. Here 'r' is just to help in clearing the leading spaces in the string

     if(s=="") 
     {
        continue;
     }
     else
     {
        while(s[r]==' ') // clears space
        {
            r++;
        }
     }

     s=s.substr(r,l-r+1);
     l=s.size();

     for(int i=0;i<l;i++) // finds position of semicolon
     {
       if(s[i]==';')
       {
        poss=i;
       }
     }

     if(poss!=-1)
     {
        s=s.substr(0,poss);
     }
     l=s.size();

     if(l==0)
     {
        continue;
     }

     for(int i=0;i<l;i++) // finds position of ":"
     {
       if(s[i]==':')
       {
        posc=i;
       }
     }

     if(posc!=-1)
     {
       string w=s.substr(0,posc);

       if(!((w[0]<='Z' && w[0]>='A')  || (w[0]<='z' && w[0]>='a')))
       {
        string op_str="Bogus Label Name at line number: ";
        flag++;
        outfile<<op_str<<PC<<endl;
        continue;
       }

       if(m3[w]==0)
       {
        m3[w]=PC;
        
        if(posc==l-1)
        {
            string op_str="Empty Label Warning at line number: ";
            outfile<<op_str<<PC<<endl;
            continue;
        }

        s=s.substr(posc+1,l-posc); // update string after clearing spaces and all
        l=s.size();
        
        int u=0,flag=0;
         
        while(s[u]==' ')
        {
            u++;
            if(u>=l)
            {
                break;
            }
        }

        s=s.substr(u,l-u+1); // update string after clearing spaces and all
        l=s.size();
       }
       else
       {
         string op_str="Duplicate Label Definition at line number: ";
         flag++;
         outfile<<op_str<<PC<<endl;
         continue;
       }
     }
     
     if(l==0)
     {
        continue;
     }

    vector<string> substrings; // stores mneumonic, value, offset

    istringstream iss(s);
    while (iss >> s) 
    {
      substrings.push_back(s);
    }
    
    // cases for different errors/ warnings

    if(substrings[0]=="br" && m2[substrings[1]]==0)
    {
      string op_str="No such Label at line number: ";
      flag++;
      outfile<<op_str<<PC<<endl;
      continue;
    }

    if(substrings[0]!="ldc" && substrings[0]!="data" && substrings[0]!="SET" && m[substrings[0]]=="")
    {
       string op_str="Bogus Mnemonic at line number: ";
       flag++;
       outfile<<op_str<<PC<<endl;
       continue;
    }

    if(substrings.size()>=3)
    {
       string op_str="Extra on End of Line at line number: ";
       flag++;
       outfile<<op_str<<PC<<endl;
       continue;
    }
    if((substrings[0]=="add" || substrings[0]=="sub" || substrings[0]=="shl" || substrings[0]=="shr" || substrings[0]=="a2sp" || substrings[0]=="sp2a" || substrings[0]=="return" || substrings[0]=="HALT") && substrings.size()==2)
    {
       string op_str="Unexpected Operand at line number: ";
       flag++;
       outfile<<op_str<<PC<<endl;
       continue;
    }
    if(!(substrings[0]=="add" || substrings[0]=="sub" || substrings[0]=="shl" || substrings[0]=="shr" || substrings[0]=="a2sp" || substrings[0]=="sp2a" || substrings[0]=="return" || substrings[0]=="HALT") && substrings.size()==1)
    {
       string op_str="Missing Operand at line number: ";
       flag++;
       outfile<<op_str<<PC<<endl;
       continue;
    }
    if(!(substrings[0]=="add" || substrings[0]=="sub" || substrings[0]=="shl" || substrings[0]=="shr" || substrings[0]=="a2sp" || substrings[0]=="sp2a" || substrings[0]=="return" || substrings[0]=="HALT"))
    {
       string op_str="Not a Number at line number: ";

       if(!(isDecimal(substrings[1]) || isOctal(substrings[1]) || isHexadecimal(substrings[1]) || isBinary(substrings[1])) && m2[substrings[1]]==0 && data[substrings[1]]=="" && SET[substrings[1]]=="" )
       {
         outfile<<op_str<<PC<<endl;
         flag++;
         continue;
       }
       
    }
   }
}

void pass1() //First pass of the two pass assembler
{
   FILE *ip;
   
   ifstream infile("input.asm"); //To read the input file consisting of code in assembly language

   ofstream outfile("error.log"); //To print the errors in the code

   string s; //To read each line of input.asm file
   PC=0;
   while(getline(infile,s) && ++PC) //Incrementing the program counter each time
   {
     int l=s.size(),posc=-1,poss=-1,r=0; //posc is the variable to store position of colon in the input string and poss stores position of semicolon in the input string. Here 'r' is just to help in clearing the leading spaces in the string

     if(s=="")
     {
        continue;
     }
     else
     {
        while(s[r]==' ') // update string after clearing spaces and all
        {
            r++;
        }
     }

     s=s.substr(r,l-r+1);
     l=s.size();

     for(int i=0;i<l;i++) // finds semicolon
     {
       if(s[i]==';')
       {
        poss=i;
       }
     }

     if(poss!=-1)
     {
        s=s.substr(0,poss);
     }
     l=s.size();

     if(l==0)
     {
        PC--;
        continue;
     }

     for(int i=0;i<l;i++) // finds colon
     {
       if(s[i]==':')
       {
        posc=i;
       }
     }
     string lab;
     if(posc!=-1)
     {
       string w=s.substr(0,posc);
       lab=w;

       if(!((w[0]<='Z' && w[0]>='A')  || (w[0]<='z' && w[0]>='a')))
       {
        continue;
       }

       if(m2[w]==0)
       {
        m2[w]=PC-1;
       
        if(posc==l-1)
        {
            PC--;
            continue;
        }

        s=s.substr(posc+1,l-posc);
        l=s.size();
        
        int u=0,flag=0;
         
        while(s[u]==' ') // clears space
        {
            u++;
            if(u>=l)
            {
                break;
            }
        }

        s=s.substr(u,l-u+1);
        l=s.size();
       }
       else
       {
         continue;
       }
     }
     
     if(l==0)
     {
        continue;
     }

    vector<string> substrings; // stores mneunomic, offset, value

    istringstream iss(s);
    while (iss >> s) 
    {
      substrings.push_back(s);
    }

    if(substrings[0]=="data")
    {
      data[lab]=substrings[1];
      continue;
    }

    if(substrings[0]=="SET")
    {
      SET[lab]=substrings[1];
    }

    if(substrings[0]!="ldc" &&  m[substrings[0]]=="")
    {
       continue;
    }
    if(substrings.size()>=3)
    {
       continue;
    }
    if((substrings[0]=="add" || substrings[0]=="sub" || substrings[0]=="shl" || substrings[0]=="shr" || substrings[0]=="a2sp" || substrings[0]=="sp2a" || substrings[0]=="return" || substrings[0]=="HALT") && substrings.size()==2)
    {
       continue;
    }
    if(!(substrings[0]=="add" || substrings[0]=="sub" || substrings[0]=="shl" || substrings[0]=="shr" || substrings[0]=="a2sp" || substrings[0]=="sp2a" || substrings[0]=="return" || substrings[0]=="HALT") && substrings.size()==1)
    {
       continue;
    }
   }
}


void pass2()
{
   FILE *ip;
   
   ifstream infile("input.asm"); //To read the input file consisting of code in assembly language

   ofstream outfile("listing.lst"); //To print the errors in the code
   
   ofstream outfile2("machine.o"); 

   string s; //To read each line of input.asm file
   PC=0;
   while(getline(infile,s) && ++PC) //Incrementing the program counter each time
   {
     int l=s.size(),posc=-1,poss=-1,r=0; //posc is the variable to store position of colon in the input string and poss stores position of semicolon in the input string. Here 'r' is just to help in clearing the leading spaces in the string

     if(s=="")
     {
        continue;
     }
     else
     {
        while(s[r]==' ') // clear spaces
        {
            r++;
        }
     }

     s=s.substr(r,l-r+1);
     l=s.size();

     for(int i=0;i<l;i++)  // finds semicolon
     {
       if(s[i]==';')
       {
        poss=i;
       }
     }

     if(poss!=-1) // clears out
     {
        s=s.substr(0,poss+1);
        for(int i=poss;i>=0;i--)
        {
          if(s[i]==';' || s[i]==' ')
          {
            s.pop_back();
          }
          else
          {
            break;
          }
        }
     }
     l=s.size();

     if(l==0)
     {
        PC--;
        continue;
     }

     for(int i=0;i<l;i++) // finds colon
     {
       if(s[i]==':')
       {
        posc=i;
       }
     }

     string w; // stores label

     if(posc!=-1)
     {
       w=s.substr(0,posc);
      
       if(m4[w]==0)
       {
        m4[w]=PC-1; // store in map the PC of label

        if(posc==l-1) // empty label case handling
        {
            outfile<<convertHexa(PC-1,8)<<"          "<<w<<":"<<endl; 
            PC--;
            continue;
        }

        s=s.substr(posc+1,l-posc);
        l=s.size();
        
        int u=0,flag=0;
         
        while(s[u]==' ') // clearing
        {
            u++;
            if(u>=l)
            {
                break;
            }
        }

        s=s.substr(u,l-u+1);
        l=s.size();
       }
       w+=": ";
     }
     
     if(l==0)
     {        
        continue;
     }

    vector<string> substrings; // stores data, offset, mneumonics

    istringstream iss(s);
    while (iss >> s) 
    {
      substrings.push_back(s);
    }

    if(substrings[0]=="data") // handles the data case
    {
      string z=substrings[1];
      if(isHexadecimal(substrings[1]))
      {
         outfile<<convertHexa(PC-1,8)<<" "<<conv8DigitHex(substrings[1])<<" "<<w<<"data"<<" "<<z<<endl;
         
         outfile2<<HexToBin(conv8DigitHex(substrings[1]))<<endl;
         continue;
      }
      else if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
         if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),32);
           outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<" "<<w<<"data"<<" "<<z<<endl;
           outfile2<<HexToBin(substrings[1])<<endl;
         }
         else
         {
           outfile<<convertHexa(PC-1,8)<<" "<<conv8DigitHex(convertHexa(stoi(substrings[1]),6))<<" "<<w<<"data"<<" "<<z<<endl;
           outfile2<<HexToBin(conv8DigitHex(convertHexa(stoi(substrings[1]),6)))<<endl;
         }
         continue;
      }
    }

    if(substrings[0]=="SET") // handles the SET case
    {
      string z=substrings[1];
      if(isHexadecimal(substrings[1]))
      {
         outfile<<convertHexa(PC-1,8)<<" "<<conv8DigitHex(substrings[1])<<" "<<w<<"SET"<<" "<<z<<endl;

         outfile2<<HexToBin(conv8DigitHex(substrings[1]))<<endl;
         continue;
      }
      else if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),32);
           outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<" "<<w<<"SET"<<" "<<z<<endl;
           outfile2<<HexToBin(substrings[1])<<endl;
         }
         else
         {
           outfile<<convertHexa(PC-1,8)<<" "<<conv8DigitHex(convertHexa(stoi(substrings[1]),6))<<" "<<w<<"SET"<<" "<<z<<endl;
           outfile2<<HexToBin(conv8DigitHex(convertHexa(stoi(substrings[1]),6)))<<endl;
         }
         continue;
      }
    }
    
    
    if(substrings[0]=="ldc") // handles the ldc case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;
         
         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      { 
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        
        if(m2[substrings[1]]<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]],24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
         outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]],6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=convertHexa(m2[substrings[1]],6)+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
        }
      }      
    }
    else if(substrings[0]=="adc") // handles the adc case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]],24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
         outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]],6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=convertHexa(m2[substrings[1]],6)+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
        }
      }
    }
    else if(substrings[0]=="ldl") // handles the ldl case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
          string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
      
    }
    else if(substrings[0]=="stl") // handles the stl case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
          string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC-1,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
    }
    else if(substrings[0]=="ldnl") // handles the ldnl case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
          string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
      
    }
    else if(substrings[0]=="stnl") // handles the stnl case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
         string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
      
    }
    else if(substrings[0]=="add") // handles the add case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="sub") // handles the sub case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="shl") // handles the shl case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="shr") // handles the shr case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="adj") // handles the adj case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]],24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
         outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]],6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;
        }
        outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

        string sx=substrings[1]+m[substrings[0]];

        outfile2<<HexToBin(sx)<<endl;
      }
      
    }
    else if(substrings[0]=="a2sp") // handles the a2sp case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="sp2a") // handles the sp2a case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="call") // handles the call case
    {
      string z=substrings[1];
     
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
          string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
    }
    else if(substrings[0]=="return") // handles the return case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
    else if(substrings[0]=="brz") // handles the brz case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
          string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
      
    }
    else if(substrings[0]=="brlz") // handles the brlz case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
         string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
      
    }
    else if(substrings[0]=="br") // handles the br case
    {
      string z=substrings[1];
      if(isDecimal(substrings[1]))
      {
        if(substrings[1][0]=='+')
        {
          substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
        }
        if(substrings[1][0]=='-')
         {
           substrings[1]=substrings[1].substr(1,substrings[1].size()-1);
           substrings[1]=convNegHex6Dig(stoi(substrings[1]),24);
         }
         else
         {
          int n=stoi(substrings[1]);
          substrings[1]=convertHexa(n,6);
         }
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else if(isHexadecimal(substrings[1]))
      {
         substrings[1]=conv6DigitHex(substrings[1]);
         outfile<<convertHexa(PC-1,8)<<" "<<substrings[1]<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

         string sx=substrings[1]+m[substrings[0]];

         outfile2<<HexToBin(sx)<<endl;
      }
      else
      {
        if(SET[substrings[1]]!="")
        {
          int x=stoi(SET[substrings[1]]);
          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(x,8)<<" "<<w<<substrings[0]<<" "<<z<<endl;

          outfile2<<HexToBin(convertHexa(x,8))<<endl;
          continue;
        }
        if(m2[substrings[1]]-PC<0)
        {
          string str=convNegHex6Dig(m2[substrings[1]]-PC,24);
          outfile<<convertHexa(PC-1,8)<<" "<<str<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
        else
        {
         string str=convertHexa(m2[substrings[1]]-PC,6);

          outfile<<convertHexa(PC-1,8)<<" "<<convertHexa(m2[substrings[1]]-PC,6)<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<z<<endl;

          string sx=str+m[substrings[0]];

          outfile2<<HexToBin(sx)<<endl;
        }
      }
      
    }
    else if(substrings[0]=="HALT") // handles the HALT case
    {
      outfile<<convertHexa(PC-1,8)<<" "<<"000000"<<m[substrings[0]]<<" "<<w<<substrings[0]<<" "<<endl;

      outfile2<<"000000000000000000000000"<<HexToBin(m[substrings[0]])<<endl;
    }
   }
}

int main()
{
   m["ldc"]="00";  // map to store opcode
   m["adc"]="01";
   m["ldl"]="02";
   m["stl"]="03";
   m["ldnl"]="04";
   m["stnl"]="05";
   m["add"]="06";
   m["sub"]="07";
   m["shl"]="08";
   m["shr"]="09";
   m["adj"]="0A";
   m["a2sp"]="0B";
   m["sp2a"]="0C";
   m["call"]="0D";
   m["return"]="0E";
   m["brz"]="0F";
   m["brlz"]="10";
   m["br"]="11";
   m["HALT"]="12";


  // Mnemonic Struct for Table_of_Inst
 
  // operand_req = 0 : No Operand
  // operand_req = 1 : Value Operand
  // operand_req = 2 : 'data'
  // operand_req = 3 : 'set'
  // operand_req = 4 : Offset Operand
  
   Table_of_Inst["ldc"]={"00",1}; // map to store instructions table,operands expected
   Table_of_Inst["adc"]={"01",1};
   Table_of_Inst["ldl"]={"02",4};
   Table_of_Inst["stl"]={"03",4};
   Table_of_Inst["ldnl"]={"04",4};
   Table_of_Inst["stnl"]={"05",4};
   Table_of_Inst["add"]={"06",0};
   Table_of_Inst["sub"]={"07",0};
   Table_of_Inst["shl"]={"08",0};
   Table_of_Inst["shr"]={"09",0};
   Table_of_Inst["adj"]={"0A",1};
   Table_of_Inst["a2sp"]={"0B",0};
   Table_of_Inst["sp2a"]={"0C",0};
   Table_of_Inst["call"]={"0D",4};
   Table_of_Inst["return"]={"0E",0};
   Table_of_Inst["brz"]={"0F",4};
   Table_of_Inst["brlz"]={"10",4};
   Table_of_Inst["br"]={"11",4};
   Table_of_Inst["HALT"]={"12",0};
   Table_of_Inst["data"]={"",2};
   Table_of_Inst["SET"]={"",3};

   pass1();  // pass1 to store all labels and check errors
   
   func(); // prints the errors in file errors.log

   if(flag==0) // No Errors Reported (Warnings are allowed)
   {
     pass2();
   }

   
  // Errors, warnings will be printed in "error.log" file by this Assembler
  // Listing file will be: "listing.lst"
  // Object file will be: "machine.o"



  // NOTE: You have to write the assembly language code in "input.asm" file as per my Assembler functioning
}