#include<bits/stdc++.h>
#define ll long long int
using namespace std;

///////////////////////////////////////

#define n 3
vector<int> calculateHammingCode(const vector<int>& data) {
    // Calculate the number of redundant bits
    int r = 0;
    while ((1 << r) < data.size() + r + 1) {
        r++;
    }

    // Create a vector to store the Hamming code
    vector<int> hammingCode(data.size() + r);

    // Initialize the Hamming code with data bits
    int j = 0;
    for (int i = 0; i < hammingCode.size(); i++) {
        if (!((i + 1) & (i))) {  // Check if i+1 is a power of 2
            hammingCode[i] = 0;  // Initialize redundant bits to 0
        } else {
            hammingCode[i] = data[j++];
        }
    }

    // Calculate the values of redundant bits
    for (int i = 0; i < r; i++) {
        int index = (1 << i) - 1;
        int parity = 0;
        for (int j = index; j < hammingCode.size(); j += (1 << (i + 1))) {
            for (int k = 0; k < (1 << i) && j + k < hammingCode.size(); k++) {
                parity ^= hammingCode[j + k];
            }
        }
        hammingCode[index] = parity;
    }

    return hammingCode;
}
bool detectError(const std::vector<int>& received, const std::vector<int>& original) {
  if (received.size() != original.size()) {
    return true; // Error: Codeword sizes don't match
  }
  
  for (int i = 0; i < original.size(); i++) {
    if (received[i] != original[i]) {
      return false; // Error: Mismatch in bits detected
    }
  }
  return true; // No errors detected
}

bool detect(bool a[n][n][n],bool pp[n][n],vector<bool>&dp,bool hp[n],vector<int>&original){
    //parity plane
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            bool p=0;
            for(int k=0;k<n;k++){
                p=p^a[i][j][k];
            }
            if(pp[i][j]!=p) return false;
        }
    }
    
    //diagonal
    int l=0;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-1;j++){
            int x=i,y=j,z=0;
            bool p=0;
            while(x<n and y<n and z<n){
                p=p^a[x++][y++][z++];
            }
            if(dp[l++]!=p) return false;
        }
    }
    for(int j=0;j<n-1;j++){
        for(int k=1;k<n-1;k++){
            int x=0,y=j,z=k;
            bool p=0;
            while(x<n and y<n and z<n){
                p=p^a[x++][y++][z++];
            }
            if(dp[l++]!=p) return false;
        }
    }
    for(int i=1;i<n-1;i++){
        for(int k=1;k<n-1;k++){
            int x=i,y=0,z=k;
            bool p=0;
            while(x<n and y<n and z<n){
                p=p^a[x++][y++][z++];
            }
            if(dp[l++]!=p) return false;
        }
    }
    //sum of plane 
    for(int k=0;k<n;k++){
        bool p=0;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                p=p^a[i][j][k];
            }
        }
        if(hp[k]!=p) return false;
    }

    //hamming code 
    vector<int>outer;
    for(int i=0;i<n;i++){
        outer.push_back(a[i][n-1][0]);
    }
    for(int j=n-2;j>=0;j--){
        outer.push_back(a[n-1][j][0]);
    }
    for(int k=1;k<n;k++){
        outer.push_back(a[n-1][0][k]);
    }
    for(int i=n-2;i>=0;i--){
        outer.push_back(a[i][0][n-1]);
    }
    for(int j=1;j<n;j++){
        outer.push_back(a[0][j][n-1]);
    }
    for(int k=n-2;k>0;k--){
        outer.push_back(a[0][n-1][k]);
    }
    vector<int>op=calculateHammingCode(outer);
    return detectError(op,original);
}
void gen(int i,int m,vector<vector<int>>&ans,vector<int>&temp,int k){
    if(i>m) return;
    if(temp.size()==k){
        ans.push_back(temp);
        return;
    }
    temp.push_back(i);
    gen(i+1,m,ans,temp,k);
    temp.pop_back();
    gen(i+1,m,ans,temp,k);
}
void generateError(bool a[n][n][n],bool pp[n][n],vector<bool>&dp,bool hp[n],vector<int>&op){
    int k=12;
    vector<vector<int>>arr;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                arr.push_back({i,j,k});
            }
        }
    }
    vector<vector<int>>kPlet;
    vector<int>temp;
    gen(0,n*n*n,kPlet,temp,k);
    for(auto x:kPlet){
        for(auto y:x){
            int i=arr[y][0],j=arr[y][1],k=arr[y][2];
            a[i][j][k]=!a[i][j][k];
        }
        detect(a,pp,dp,hp,op);
        if(detect(a,pp,dp,hp,op)){
            cout<<"No error\n";
        }
        
        //back to original form 
        for(auto y:x){
            int i=arr[y][0],j=arr[y][1],k=arr[y][2];
            a[i][j][k]=!a[i][j][k];
        }
    }
    cout<<"Finished\n";
}
void encode(bool a[][n][n]){
    //parity generations 
    
    //parity plane
    bool pp[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            bool p=0;
            for(int k=0;k<n;k++){
                p=p^a[i][j][k];
            }
            pp[i][j]=p;
        }
    }
    
    //diagonal
    vector<bool>dp;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-1;j++){
            int x=i,y=j,z=0;
            bool p=0;
            while(x<n and y<n and z<n){
                p=p^a[x++][y++][z++];
            }
            dp.push_back(p);
        }
    }
    for(int j=0;j<n-1;j++){
        for(int k=1;k<n-1;k++){
            int x=0,y=j,z=k;
            bool p=0;
            while(x<n and y<n and z<n){
                p=p^a[x++][y++][z++];
            }
            dp.push_back(p);
        }
    }
    for(int i=1;i<n-1;i++){
        for(int k=1;k<n-1;k++){
            int x=i,y=0,z=k;
            bool p=0;
            while(x<n and y<n and z<n){
                p=p^a[x++][y++][z++];
            }
            dp.push_back(p);
        }
    }
    //sum of plane 
    bool hp[n];
    for(int k=0;k<n;k++){
        bool p=0;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                p=p^a[i][j][k];
            }
        }
        hp[k]=p;
    }
    //hamming code 
    vector<int>outer;
    for(int i=0;i<n;i++){
        outer.push_back(a[i][n-1][0]);
    }
    for(int j=n-2;j>=0;j--){
        outer.push_back(a[n-1][j][0]);
    }
    for(int k=1;k<n;k++){
        outer.push_back(a[n-1][0][k]);
    }
    for(int i=n-2;i>=0;i--){
        outer.push_back(a[i][0][n-1]);
    }
    for(int j=1;j<n;j++){
        outer.push_back(a[0][j][n-1]);
    }
    for(int k=n-2;k>0;k--){
        outer.push_back(a[0][n-1][k]);
    }
    vector<int>op=calculateHammingCode(outer);
    generateError(a,pp,dp,hp,op);
    
}


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    bool  a[n][n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                a[i][j][k]=rand();
            }
        }
    }
    encode(a);
    // generateError(a);
    // vector<int>arr={1,0,1,1};
    // vector<int>temp=calculateHammingCode(arr);
    // detectError(temp);
    return 0;
}