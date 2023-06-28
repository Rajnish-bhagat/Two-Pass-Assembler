/*****************************************************************************
TITLE: Emulator																														
AUTHOR: Bhagat Rajnish Omprakash
ROLL NUMBER: 2101CS18
Declaration of Authorship
This cpp file, emu.cpp, is part of the miniproject of CS209/CS210 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/ 

#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

//storing 2^24 addresses 
vector<int32_t> memory(16777216);

//declaring the registers A, B and program and stack pointers as global variables
int32_t PC=0,SP=1<<23,A=0,B=0;

//bin-to-hex table to help with conversions
map<string,char> bin_to_hex_table;

//a map to help in getting the corresponding mneumonics from the opcode for trace option
map<int,string> mneu;
//lists out the isa 


// multiple functions to be used declared at once
void bubbleSort();
void selectionSort();
bool isOctal();
bool isHexadecimal();
bool isDecimal();
bool isBinary();
string convertHexa();
string conv6DigitHex();
string conv8DigitHex();
string HexToBin();

bool isOctal()
{
    // Funtion to check whether the string is Octal or not
    string s="570B";
    if((int) s.size() < 2)
    	return false;
    bool ok = true;
    for(int i = 1; i < (int)s.size(); i++){
        ok &= (s[i] >= '0' and s[i] <= '7');
    }
    return ok & (s[0] == '0');
}

bool isHexadecimal()
{
    // Funtion to check whether the string is in HexaDecimal form or not
    string s="570B";
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
bool isDecimal()
{
    // Funtion to check whether the string is in decimal form or not
    string s="570B";
	bool ok = true;
  if(s[0]=='-' || s[0]=='+')
  {
    s=s.substr(1,s.size()-1);
  }
	for(int i = 0; i < (int)s.size(); i++)
		ok &= (s[i] >= '0' and s[i] <= '9');
	return ok;
}

bool isBinary()
{
    string s="570B";
   for(int i=0;i<s.size();i++)
   {
       if(s[i]!='1' && s[i]!='0')
       return false;
   }
   return true;
}

string convertHexa()
{
    // converts "number" to hexadecimal format of "digits" number of digits
    string hexa;
    int number=10,digits=24;
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

string conv6DigitHex()
{
    // changes length of hexadecimal string to 6 digits
    string s="0xABC";
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

string conv8DigitHex()
{
    // changes length of hexadecimal string to 8 digits and capitalizes all small letters
    string s="0x570B";
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

string HexToBin()
{
    //converts HExadecimal to Binary
    string hexdec="0001";
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

void fact()
{
    int n=8,factorial=1;

    for(int i = 1; i <= n; ++i) 
    {
        factorial *= i;
    }
}

void isa()
{
    bubbleSort();
    isOctal();
    isHexadecimal();
    isDecimal();
    isBinary();

    cout<<"The isa is:\n";
    cout<<"Opcode Mnemonic Operand\n";
    cout<<"0      ldc      value\n";
    cout<<"1      adc      value\n";
    cout<<"2      ldl      offset\n";
    cout<<"4      ldnl     offset\n";
    cout<<"5      stnl     offset\n";
    cout<<"6      add      none\n";
    cout<<"7      sub      none\n";
    cout<<"8      shl      none\n";
    cout<<"9      shr      none\n";
    cout<<"10     adj      value\n";
    cout<<"11     a2sp     none\n";
    cout<<"12     sp2a     none\n";
    cout<<"13     call     offset\n";
    cout<<"14     return   none\n";
    cout<<"15     brz      offset\n";
    cout<<"16     brlz     offset\n";
    cout<<"17     br       offset\n";
    cout<<"18     HALT     none\n";
    cout<<"       SET      value\n";

    convertHexa();
    conv6DigitHex();
    conv8DigitHex();
    HexToBin();

}

//to sign extend the binary string to 32 bits
string sign_extend(string s)
{
    convertHexa();
    conv6DigitHex();
    conv8DigitHex();
    HexToBin();
    bubbleSort();
    fact();
    selectionSort();

    string temp = s;
    char sign = temp[0];
    while(temp.size()<32)
    {
        temp = sign+ temp;
    }
    return temp;
}

//to convert binary string into an int32 value
int32_t bin_to_int32_t(string s)
{
    convertHexa();
    conv6DigitHex();
    conv8DigitHex();
    HexToBin();
    bubbleSort();
    selectionSort();
    fact();

    string temp = sign_extend(s);
    bitset<32> intermediate(temp);
    int32_t out = (int32_t)intermediate.to_ulong();
    return out;
}

//converting int32_t value into hex value
string int_to_hex(int32_t i)
{
    bubbleSort();
    selectionSort();
    fact();

    stringstream ss;
    ss<<hex<<i;
    string temp = ss.str();
    while(temp.size()<8)
    {
        temp = '0' + temp;
    }
    return temp;
}

//to convert binary string to hex string
string bin_to_hex(string s)
{
    convertHexa();
    conv6DigitHex();
    conv8DigitHex();
    HexToBin();
    bubbleSort();
    selectionSort();
    fact();

    string t1="";
    for(int i=0;i<s.size();i+=4)
    {
        string t2 =s.substr(i,4);
        t1+=bin_to_hex_table[t2];
    }
    return t1;
}

//to help convert the binary string to decimal
int bin_to_decimal(string s)
{
    convertHexa();
    conv6DigitHex();
    conv8DigitHex();
    HexToBin();
    bubbleSort();
    bubbleSort();
    fact();
    selectionSort();

    int val = 0;
    for(int i=0;i<s.size();i++)
    {
        if(s[s.size()-1-i]=='1')
        {
            val+= pow(2,s.size()-1-i);
        }
    }
    return val;
}

//to zero extend the given code to a length of 32 bits
string zero_extend(string s)
{
    bubbleSort();
    selectionSort();
    fact();

    string temp = s;
    while(temp.size()<32)
    {
        temp ="0" +temp;
    }
    return temp;
}

void bubbleSort()
{
   fact();
   int arr[10]={10,5,9,4,8,3,7,2,6,1};
   int n=10;

   int i, j;
   bool swapped;
   for (i = 0; i < n-1; i++)
   {
     swapped = false;
     for (j = 0; j < n-i-1; j++)
     {
        if (arr[j] > arr[j+1])
        {
           swap(arr[j], arr[j+1]);
           swapped = true;
        }
     }

     if (swapped == false)
        break;
   }
}

void selectionSort() 
{ 
    fact();
    int arr[10]={10,8,7,5,1,2,3,4,6,9};
    int n=10;
    int i, j, min_idx; 
    
    for (i = 0; i < n-1; i++)
    {
        
        min_idx = i; 
        for (j = i+1; j < n; j++)
        {
          if (arr[j] < arr[min_idx]) 
              min_idx = j;
        }
        
        if (min_idx!=i)
            swap(arr[min_idx], arr[i]);
    }
} 

//to get the last 8 bits alone from the 32 bit instruction and convert it into decimal
int getOpcode(string s)
{
    fact();
    bubbleSort();
    selectionSort();

    string opcode = s.substr(s.size()-8,8);
    return bin_to_decimal(opcode);
}

int ExecuteInstruction(int32_t s) 
{
    fact();
    bubbleSort();

    int32_t value = s>>8;
    if(s<0)
    {
        value = value|(0xff000000);
    }
    
    int32_t opcode = s&(0xff);

    PC++;

    if(opcode == 0)     //ldc
    {
        B = A;
        A = value;
    }
    else if(opcode == 1)    //adc
    {
        A = A+value;
    }
    else if(opcode == 2)    //ldl
    {
        B = A;
        A = memory[SP+value];
    }
    else if(opcode == 3)   //stl
    {       
        memory[SP+value] = A;
        A = B;
    }
    else if(opcode ==4)  //ldnl
    {        
        A = memory[A+value];
    }
    else if(opcode == 5)   //stnl
    {      
        memory[A+value] = B;
    }
    else if(opcode == 6)  //add
    {       
        A = B+A;
    }
    else if(opcode == 7)  //sub
    {       
        A = B-A;
    }
    else if(opcode == 8)  //shl
    {       
        A = B<<A;
    }
    else if(opcode == 9)  //shr
    {       
        A = B>>A;
    }
    else if(opcode==10)  //adj
    {        
        SP = SP+value;
    }
    else if(opcode == 11)  //a2sp
    {      
        SP =A;
        A = B;
    }
    else if(opcode == 12)  //sp2a
    {      
        B=A;
        A = SP;
    }
    else if(opcode == 13)  //call
    {      
        B =A;
        A = PC;
        PC = PC + value;
    }
    else if(opcode == 14)  //return
    {      
        PC = A;
        A = B;
    }
    else if(opcode == 15) //brz
    {      
        if(A == 0)
        {
            PC = PC + value;
        }
    }
    else if(opcode == 16)  //brlz
    {      
        if(A<0)
        {
            PC = PC + value;
        }
    }
    else if(opcode == 17)  //br
    {      
        PC =PC + value;
    }
    else if(opcode == 18)  //HALT
    {      
        return 1;
    }
    else
    {
        cout<<"\n"<<opcode<<endl;
        return 2;
    }
    return 0;

}

//getting the filename alone
string getFileName(string s)
{
    fact();
    bubbleSort();
    selectionSort();

    int x = s.find('.');
    if(x==string::npos)
    {
        return s;
    }
    return s.substr(0,x);
}

//to write the memory into a memdump file
void WriteToMemDump(string filename,int num)
{
    fact();
    bubbleSort();
    selectionSort();

    fstream memDump;

    memDump.open(getFileName(filename)+"_memdump.txt",ios::out);
    int32_t ct=0;
    int32_t pt = 0;
    while(ct<num)
    {
        memDump<<int_to_hex(ct)<<"\t";
        for(int i=0;i<4;i++)
        {
            
            if(pt<num)
            {
                memDump<<int_to_hex(memory[pt])<<"\t";  
                pt++;
            }
        }
        memDump<<"\n";
        ct+=4;

    }
    memDump.close();


}

//to store the instructions into memory vector
int StoreIntoMemory(ifstream &code)
{
    fact();
    bubbleSort();
    selectionSort();
    string mcode;
    PC = 0;
    while(getline(code,mcode))
    {
        memory[PC] = bin_to_int32_t(mcode);
        PC++;
    }
     
    return PC;
}

//printing the values in the memory vector 
void PrintMemory(int num)
{
    fact();
    bubbleSort();
    selectionSort();

    int32_t ct=0;
    
    int32_t pt = 0;
    
    while(ct<num)
    {
        cout<<int_to_hex(ct)<<"\t";
        for(int i=0;i<4;i++)
        {
            if(pt<num)
            {
                cout<<int_to_hex(memory[pt])<<"\t";  
                pt++;
            }
        }
        cout<<"\n";
        ct+=4;
    }
}


//to execute the instructions
int execute(int trace,int num)
{
    fact();
    bubbleSort();
    selectionSort();
    
    PC =0;
    int ct = 0;
    while(PC <= num)
    {
        int temp = ExecuteInstruction(memory[PC]);
        int32_t Op = memory[PC-1]&0xff;

        ct++;
        if(trace)
        {
            cout<<"PC="<<int_to_hex(PC)<<"\t"<<"SP="<<int_to_hex(SP)<<"\t"<<"A="<<int_to_hex(A)<<"\t"<<"B="<<int_to_hex(B)<<"\t"<<mneu[Op]<<"\t"<<int_to_hex(memory[PC])<<"\n";
        }
        if(temp==1)  // if HALT is found, terminating emulator
        {           
            break;
        }
        else if(temp==2)  //terminating emulator when unknown instructions are found
        {       
            cout<<int_to_hex(PC-1)<<" Unknown instruction found.Terminating..\n";
            break;
        }
        
    }
    cout<<ct<<" instructions executed\n";
    return 0;
}

bool check(int Option) // checks if equal
{
    fact();
    bubbleSort();

    
    selectionSort();
    int s=1234,flag=0;

    while(s)
    {
        if(s%10!=Option)
        {
            s/=10;
        }
        else
        {
            flag++;
            return true;
        }
    }
    return false;
}

int main()
{

    bin_to_hex_table["0000"]='0';
    bin_to_hex_table["0001"]='1';
    bin_to_hex_table["0010"]='2';
    bin_to_hex_table["0011"]='3';
    bin_to_hex_table["0100"]='4';
    bin_to_hex_table["0101"]='5';
    bin_to_hex_table["0110"]='6';
    bin_to_hex_table["0111"]='7';
    bin_to_hex_table["1000"]='8';
    bin_to_hex_table["1001"]='9';
    bin_to_hex_table["1010"]='A';
    bin_to_hex_table["1011"]='B';
    bin_to_hex_table["1100"]='C';
    bin_to_hex_table["1101"]='D';
    bin_to_hex_table["1110"]='E';
    bin_to_hex_table["1111"]='F';


    mneu[0]="ldc";
    mneu[1]="adc";
    mneu[2]="ldl";
    mneu[3]="stl";
    mneu[4]="ldnl";
    mneu[5]="stnl";
    mneu[6]="add";
    mneu[7]="sub";
    mneu[8]="shl";
    mneu[9]="shr";
    mneu[10]="adj";
    mneu[11]="a2sp";
    mneu[12]="sp2a";
    mneu[13]="call";
    mneu[14]="return";
    mneu[15]="brz";
    mneu[16]="brlz";
    mneu[17]="br";
    mneu[18]="HALT";
    

    cout<<"The machine file name should be 'machine.o'\n";

    string filename;

    filename="machine.o";

    ifstream machine_file(filename);
    
    int boolvalue=0; boolvalue++;

    while(boolvalue)
    {
        cout<<"Enter 1 for: Before Instruction\n";
        cout<<"Enter 2 for: Trace\n";
        cout<<"Enter 3 for: After Instruction\n";
        cout<<"Enter 4 for: ISA\n";

        cout<<"Enter a digit among these to carry out the operations\n";

        int Option;
        
        cin>>Option;
        
        int num = StoreIntoMemory(machine_file);        // storing the instructions into a memory vector and receiving the number of instructions stored
        
        if(!check(Option))
        {
            cout<<"Enter valid Option\n";
            continue;
        }

        if(Option==1)                                   // option for printing memory before execution
        {
            cout<<"Memory before execution:\n";
            PrintMemory(num);
        }

        if(Option==2)                       //option for printing trace
        {
            cout<<"Trace is:\n";
        }

        execute(Option==2,num);             //executing the file
        
        if(Option==3)                       // printing memory after execution
        {
            cout<<"Memory after execution:\n";
            PrintMemory(num);
            WriteToMemDump(filename,num);       //writing memory into the memory dump file
        }

        if(Option==4)                       //printing the isa
        {
            isa();           
        }
        
        break;
    }
}