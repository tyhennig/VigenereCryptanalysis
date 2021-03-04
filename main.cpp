#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;



float LETTERFREQ[] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,  // A-G
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,  // H-N
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,  // O-U
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074}; //V-Z
string keyGuess = "";



int main()
{
    string s =  "TSMVMMPPCWCZUGXHPECPRFAUEIOBQWPPIMSFXIPCTSQPKSZNULOPACRDDPKTSLVFWELTKRGHIZSFNIDFARMUENOSKRGDIPHWSGVLEDMCMSMWKPIYOJSTLVFAHPBJIRAQIWHLDGAIYOUX";
    //s = "bkeijmgykkaclfzegecwhpxcgkmuitmattyxpelzgeessxympvmmoitbnsiimftwdxqdexxgxrfmqfvvbdtxnbjwafkimittsdizdgyhfysdiumfpwgxateclrfjwmmprjnppmkpmcovwdzvkayesexgytrkrtgleyc";
    for(char& c : s){
        c = toupper(c);
    }
    int numTests = 15; //set the max period you want to test
    int indexOfMaxIC = 0;
    
    vector<float> icVals;
    
    for(int i = 2; i <= numTests; i++){ //test for each period starting with 2 up until the max you set
        
        vector<string> seqVec(i); //vector to hold the sequence strings
        
        for(int j = 0; j < s.length(); j++){//split the cipher text into the vector using %(period we are currently testing)
            seqVec[j%i] += s[j];
        }
        
        float indexC = 0.0f;
        for(int k = 0; k < i; k++){ //iterate through each sequence string to calculate the IC
           float letterSum = 0.0f;
           int alphabet[26] = {0}; //array to hold the # of each letter found
           
           for(int a = 0; a < seqVec[k].size(); a++){//for every letter in the current sequence string

                alphabet[(seqVec[k][a] - 'A')]++; //increments the index where that letter is represented (if A => index 0++, B => index 1++ etc..)
           }
           
           for(int a = 0; a < 26; a++){//for every letter in the alphabet (english in this case)
               
               letterSum += (alphabet[a]*(alphabet[a] - 1)); //formula for IC
           }
           indexC += letterSum / (seqVec[k].size()*(seqVec[k].size() - 1));
        }
        indexC /= i; //Average the IC for all the sequence strings
        icVals.push_back(indexC);
        cout << "IC for period of " << i << ": " << indexC << endl;
    }
    for(int i = 0; i < icVals.size(); i++)
        if(icVals[i] > icVals[indexOfMaxIC])
            indexOfMaxIC = i;
    cout << indexOfMaxIC << endl;
    

    for(int i = 0; i < indexOfMaxIC + 2; i++){ //for every "letter" in the key
        string strSameKey = "";

        
        for(int j = i; j < s.length(); j = j+5){//find the letters that use the same key
            strSameKey += s[j];
        }
        cout << strSameKey << endl;
        
        float chiSquare[26] = {0.0f};
        for(int a = 0; a < 26; a++){ //calculate the ChiSquare and increment every letter + 1 each time
            int alphabet[26] = {0};

            cout << "Key: " << a << " Text: " << strSameKey;
            for(char& c : strSameKey){//count instances of each letter and store in alphabet[]
                int cIndex = (c-'A');
                alphabet[cIndex]++;
                
                cIndex = ((cIndex + 25) % 26);
                
                c = (cIndex + 'A');
                
                //stringCounter++;
            }
            
            int letterIndex = 0;
            for(int k : alphabet){ //calculate the index of coincidence for the given string and store it in chiSquare[]
                float expectedCount = LETTERFREQ[letterIndex++]*strSameKey.length();
                chiSquare[a] += (pow((((float)k) - expectedCount), 2) / expectedCount);
            }
            
            
            cout << "Chi Square: " << chiSquare[a] << "  Period: " << i << endl;
        }
     
     float minChi = chiSquare[0];
     int chiCounter = 0;
     int minIndex = 0;
     for(float f : chiSquare){ //find the minimum chisquare for this period
         if(f < minChi) {
            minChi = f;
            minIndex = chiCounter;
         }
        chiCounter++;
     }   
     cout << "Minimum CHI: " << minChi << " at key " << minIndex << endl;
     keyGuess += ('A' + minIndex);
    }

    cout << "Best key guess is: " << keyGuess << endl;
    
    for(int i = 0; i < s.length(); i++){ //Decrypt the ciphertext using the best key guess
        s[i] = ((((s[i]) - (keyGuess[i%keyGuess.length()]))+26)%26) + 'A';
        
    }
    cout << s << endl;
    
}
