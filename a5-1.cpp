// a5.cpp

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Kunpeng Wang>
// St.# : <301326336>
// Email: <kunpengw@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// You are only allowed to include these files --- don't include anything else!
// If you think it's necessary to include another file, please speak to the
// instructor.
//

 #include "cmpt_error.h"
 #include <iostream>
 #include <string>
 #include <fstream>   // for reading/writing text files

using namespace std;

//build a class for compile hash table

class Table{
private:

  int capacity;
  int total;



  ///////////////////// math function //////////////////////

  int power(int num,int power){
    int result=num;
    for(int i=0;i<power-1;i++){
      result*=num;
    }
    return result;
  }
  int abs(int num){
    if(num<0){
      num=-1*num;
    }
    return num;
  }



public:
  string* arr;   //arr for save data

  //constructor
  Table(){
    //assume there are tens of thousands of word
    capacity=10000;
    total=0;
    arr=new string[capacity];
  }

  //destructor
  ~Table(){
    delete []arr;

  }

  int getCap(){
    return capacity;
  }
  int getTot(){
    return total;
  }

  //use to add word to hash table
  void add(string word){
    //first check if the load fact is above 0.5
    double loadFact=static_cast<double>(total)/capacity;
    if(loadFact>0.5){
      capacity=capacity*2;
      string* temp =new string[capacity];

      //move arr data to temp
      for(int i=0;i<capacity/2;i++){
        int rehashIntWord=hashCode(arr[i]);
        int rehashArrIndex=compressionFunction(rehashIntWord);
        // we need leave temp[0] as empty, so we set temp[0] to temp[1]
        if(rehashArrIndex==0){
          rehashArrIndex=1;
        }

        //insert arr data to temp
        if(temp[rehashArrIndex].empty()){
          temp[rehashArrIndex]=arr[i];
        }
        else{
          int j=0;
          int insertCheck=0;
          while(rehashArrIndex+power(j,2)<capacity){
            j++;
            if(temp[rehashArrIndex+power(j,2)].empty()){
              temp[rehashArrIndex+power(j,2)]=arr[i];
              insertCheck=1;
              break;
            }
          }
          if(insertCheck!=1){
            int i=0;
            while(power(i,2)<rehashArrIndex){
              i++;
              if(temp[power(j,2)].empty()){
                temp[power(j,2)]=arr[i];
                break;
              }
            }
          }
        }
      }
      string* del=arr;
      arr=temp;
      delete []del;
    }

    // then insert new word to arr
    //first do hash code to the insert word
    int intWord=hashCode(word);
    //second do compression function
    int arrIndex=compressionFunction(intWord);
    //we leave arr[0] as empty so move it to arr[1]
    if(arrIndex==0){
      arrIndex=1;
    }
    // check if this location is Data be remove(DR)
    if(arr[arrIndex]=="DR"){
      arr[arrIndex]=word;
    }
    //check if this location is full
    //if gull do quadrtic hashing
    if(arr[arrIndex].empty()){
      arr[arrIndex]=word;
    }
    else{
      int j=0;
      int insertCheck=0;
      while(arrIndex+power(j,2)<capacity){
        j++;
        if(arr[arrIndex+power(j,2)].empty()){
          arr[arrIndex+power(j,2)]=word;
          insertCheck=1;
          break;
        }
      }
      if(insertCheck!=1){
        int i=0;
        while(power(i,2)<arrIndex){
          i++;
          if(arr[power(j,2)].empty()){
            arr[power(j,2)]=word;
            break;
          }
        }
      }
    }
    total++;
  }

  //remove the word from hash table
  void remove(string word){
    //find the location of the word
    int loc=find(word);
    //remove from hash table
    if(loc!=0){
      arr[loc]="DR";     //which mean this location have a data and now be removed
    }
  }

  // find the word from the hasd table, and return the index of that word
  int find(string word){

    int intWord=hashCode(word);
    int arrIndex=compressionFunction(intWord);

    // check if this location is Data be remove(DR)
    if(arr[arrIndex]=="DR"){
      return 0;
    }
    if(arr[arrIndex]==word){
      return arrIndex;
    }
    else if(arr[arrIndex]!=word){
      int j=0;
      while(arrIndex+power(j,2)<capacity){
        if(arr[arrIndex+power(j,2)]==word){
          return arrIndex+power(j,2);
        }
        j++;
      }
      int i=0;
      while(power(i,2)<arrIndex){
        i++;
        if(arr[power(i,2)]==word){
          return power(i,2);
        }
      }
    }
    return 0;
  }




  ///////////////// help function ////////////////

  //hash code make string to int
  int hashCode(string word){
    //from text book, we take a=33
    int result=0;
    int len=word.length();
    len-=1;
    while(len>=0){
      result+=word[len]*power(33,len);
      len--;
    }
    return result;
  }

  //compression function compute the mod of int number of string
  int compressionFunction(int key){
    //do a compression function, we use pivision;
    int index;
    index=abs(key)%capacity;
    return index;
  }

};



////////////////////////////////////
///        main function        ///
///////////////////////////////////

int main(){

  cout<<"welcom to my spell checker pro"<<endl;

  string str;
  Table maindict;
  ifstream infileM;
  infileM.open("maindict.txt");
  while(infileM>>str){
    maindict.add(str);
  }

  Table addedwords;
  ifstream infileA;
  infileA.open("addedwords.txt");
  while(infileA>>str){
    addedwords.add(str);
  }


  while(1){
    //cin command and word
    string comd;
    cin>>comd;
    if(comd=="quit"||comd=="stop"||comd=="end"||comd=="done"){
      break;
    }
    string cinWord;
    cin>>cinWord;


    //recognized the commend
   if(comd=="check"){
      if(maindict.find(cinWord)!=0){
        cout<<"ok"<<endl;
      }
      else if(addedwords.find(cinWord)!=0){
        cout<<"ok"<<endl;
      }
      else{
        cout<<"don't know "<<"\""<< cinWord<<"\""<<endl;
      }
    }

    else if(comd=="add"){
      //add to addewords.txt
      ofstream ofileA;
      ofileA.open("addedwords.txt",ios::app);
      ofileA<<cinWord<<endl;
      //add to hash table of addewords
      addedwords.add(cinWord);
      cout<<"\""<<cinWord<<"\""<<" added to the dictionary"<<endl;
    }

    else if(comd=="remove"){
      //check which hash table find the word
      if(maindict.find(cinWord)!=0){
        cout<<" can't remove "<<"\""<<cinWord<<"\""<<": it's in the main dictionary";
      }
      else if(addedwords.find(cinWord)!=0){
        //remove from hash table first
        addedwords.remove(cinWord);
        cout<<"\""<<cinWord<<"\""<<"removed"<<endl;
      }
      else{
        cout<<"\""<<cinWord<<"\""<<" is not in maindict or addedwords"<<endl;
      }

    }

    else if(comd=="checkfile"){
      cout<<"the words which cannot find in maindict.txt and addedwords.txt is "<<endl;
      Table temp;
      string str;
      ifstream infileT;
      string sum=cinWord+".txt";
      infileT.open(sum);
      int lin=0;
      while(infileT>>str){
        int check=0;
        if(maindict.find(str)==0){
          check=1;
        }
        if(addedwords.find(str)!=0){
          check=0;
        }
        if(check==1){
          cout<<lin<<" "<<str<<endl;
        }
        lin++;
      }
    }

    else if(comd=="suggest"){
      cout<<"we suggest some words for you"<<endl;
      int count=0;
      int out=0;
      string origin=cinWord;
      for(int i=0;i<cinWord.length();i++){
        for(int j=0;j<26;j++){
          cinWord[i]='a'+j;
          if(maindict.find(cinWord)!=0){
            if(cinWord!=origin){
              cout<<cinWord<<endl;
              count++;
            }
          }
          if(addedwords.find(cinWord)!=0){
            if(cinWord!=origin){
              cout<<cinWord<<endl;
              count++;
            }
          }
          if(count>5){
            out=1;
            break;
          }
        }
        if(out==1){
          break;
        }
      }
    }

    cout<<endl;
  }
  return 0;
}
