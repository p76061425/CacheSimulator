#include <iostream>
#include <string>
#include <cstdlib>
#include<fstream>
#include <math.h>
#include <vector>
#include <sstream>
#include <bitset>
#include <cstdlib> 
#include <ctime>  
using namespace std;

class Cache_block {	
	
	public:
		Cache_block():valid(0) ,tag(0) {} ;
		int valid;
		unsigned long long tag;

};

class Cache_set{
	public:
		Cache_set(){
            
		};
		vector<Cache_block> blocks;
		
};

class Cache{
	
	public:
		Cache(unsigned int assoc, unsigned int index_num, string policy_str, unsigned int block_size):
			sets(index_num),assoc(assoc),index_num(index_num),policy_str(policy_str),miss_cnt(0),hit_cnt(0),total_cnt(0),
            block_size(block_size),r_miss_cnt(0),w_miss_cnt(0),r_cnt(0),w_cnt(0){                
		};
		vector<Cache_set> sets;
		unsigned int assoc;
		unsigned int index_num;
		string policy_str;
        unsigned int miss_cnt;
        unsigned int hit_cnt;
		unsigned int total_cnt;
        unsigned int block_size;
        unsigned int r_miss_cnt;
        unsigned int w_miss_cnt;
        unsigned int r_cnt;
        unsigned int w_cnt;
        
        void access(char op, string addr){
            unsigned long long hex_addr; 
            std::stringstream ss;
            ss <<std::hex <<addr;
            ss>>hex_addr;
        
            unsigned long long  tag;
            unsigned long long  index;
            unsigned long long block_addr;
            
            block_addr = int(hex_addr/this->block_size);
            tag = int(block_addr/this->index_num);
            index = block_addr%this->index_num; 
            
            bool full = false;
            int count = 0; 
            
            ////測index有沒有滿 
            //if(sets[index].blocks.size() == this->assoc){
            //    full = true;
            //}
            
            bool miss = true;
            this->total_cnt++;
            if(op == 'r'){
                this->r_cnt++;
            }else if(op == 'w'){
                this->w_cnt++;
            }else{
                cout<< "\nERROR OP" << op <<"\n";
            }
            
            for(int i=0; i<sets[index].blocks.size(); i++  ){
                
                //hit
                if(sets[index].blocks[i].valid && sets[index].blocks[i].tag == tag){
                    //cout<<"hit"<<endl;
                    this->hit_cnt++;
                    miss = false;
                    
                    Cache_block temp = sets[index].blocks[i];
                    sets[index].blocks.erase(sets[index].blocks.begin()+i);
                    sets[index].blocks.insert(sets[index].blocks.end(),temp);
                    break;
                }  
            }      
              
            if(miss){
                this->miss_cnt++;
                if(op == 'r'){
                    this->r_miss_cnt++;
                }else if(op == 'w'){
                    this->w_miss_cnt++;
                }else{
                    cout<< "\nERROR OP" << op <<"\n";
                }
                
                if(sets[index].blocks.size() < this->assoc){
                    Cache_block temp;
                    temp.valid = 1;
                    temp.tag = tag;
                    sets[index].blocks.insert(sets[index].blocks.end() ,temp);
                }
                else{ //index full
                    Cache_block temp;
                    temp.valid = 1;
                    temp.tag = tag;
                    if(policy_str == "l"){
                        //LRU
                        sets[index].blocks.erase(sets[index].blocks.begin());
                    }
                    else if(policy_str == "r"){
                        //random
                        int min = 0;
                        int max = sets[index].blocks.size();
                        //int x = rand() % (max - min + 1) + min;
                        int x = rand()% max+1;
                        //cout<< "\n" << x << "\n";
                        sets[index].blocks.erase(sets[index].blocks.begin()+x);
                        
                    }
                    sets[index].blocks.insert(sets[index].blocks.end() ,temp);                        
                }
            }  
			return;
		}
};

int main(int argc , char* argv[]){
	srand( time(NULL) );
    
    //string cache_size_str = "2048";
    //string assoc_str = "64";
    //string block_size_str = "64";
    //string lru_str = "1";
    
    string cache_size_str = argv[1];
    string assoc_str = argv[2];
    string block_size_str = argv[3];
    string policy_str = argv[4];
    string benchmarkFile_str = argv[5];
    
    string buffer;
    
	unsigned int cache_size = atoi(cache_size_str.c_str()) * 1024;
	unsigned int block_size = atoi(block_size_str.c_str());
	unsigned int assoc = atoi(assoc_str.c_str());
	//unsigned int lru = atoi(lru_str.c_str()); 
    
	unsigned int block_num = cache_size/block_size; 
	unsigned int index_num = block_num/assoc;
	unsigned int offset_size = log2(block_size);
	unsigned int index_size = log2(index_num);
	unsigned int tag_size = 64 - index_size - offset_size;
	
	cout<< "\noffset_size:"<<offset_size<<'\n';
	cout<< "index_size: "<<index_size<<'\n';	
	cout<< "tag_size:   "<<tag_size<<'\n';
	cout<< "cache_size:" << cache_size <<'\n';
	cout<< "block_size:" << block_size <<'\n';
	cout<< "block_num:" << block_num <<'\n';
    cout<< "index_num:" << index_num <<'\n';
    cout<< "offset_size:" << offset_size <<'\n';
    cout<< "index_size:" << index_size <<'\n';
    cout<< "tag_size:" << tag_size <<'\n';
    cout<< "assoc:" << assoc <<'\n';
    
	Cache cache(assoc,index_num,policy_str,block_size);
    fstream fin;
    //fin.open("429.mcf-184B.trace.txt",ios::in);
    fin.open(benchmarkFile_str.c_str(), ios::in);	
    
    string line;
    while(getline(fin,line)){
        char op = line[0];
        string addr = line.substr(2,-1);
        cache.access(op,addr);
    }
    
    //cout<< line;
    //while(getline(std::cin,line)){
    //    char op = line[0];
    //    string addr = line.substr(2,-1);
    //    cache.access(op,addr);
    //}
    
    cout<<"miss_cnt:"<<cache.miss_cnt <<"\n";
    cout<<"hit_cnt:"<<cache.hit_cnt <<"\n";   
    cout<<"total_cnt:"<<cache.total_cnt <<"\n"; 
    cout<<"r_miss_cnt:"<<cache.r_miss_cnt <<"\n"; 
    cout<<"w_miss_cnt:"<<cache.w_miss_cnt <<"\n"; 
    cout<<"r_cnt:"<<cache.r_cnt <<"\n"; 
    cout<<"w_cnt:"<<cache.w_cnt <<"\n"; 
    
    float percent_miss = float(cache.miss_cnt)/float(cache.total_cnt);
    float percent_r_miss = float(cache.r_miss_cnt)/float(cache.r_cnt);
    float percent_w_miss = float(cache.w_miss_cnt)/float(cache.w_cnt);
    
    cout<<"\n========================================\n"; 
    cout<<"1.total number of misses:" << cache.miss_cnt <<'\n'; 
    cout<<"2.percentage of misses:" << percent_miss*100 <<"%\n"; 
    cout<<"3.total number of read misses:" << cache.r_miss_cnt<<'\n'; 
    cout<<"4.percentage of read misses:" << percent_r_miss*100 <<"%\n"; 
    cout<<"3.total number of write misses:" << cache.w_miss_cnt <<'\n'; 
    cout<<"4.percentage of write misses:" << percent_w_miss*100 <<"%"; 
    cout<<"\n========================================\n"; 
    
	
}


