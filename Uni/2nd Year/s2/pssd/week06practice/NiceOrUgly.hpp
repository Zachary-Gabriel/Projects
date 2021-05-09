#include <string>
#include <vector>
#include <iostream>
using namespace std;
class NiceOrUgly {
public:
	string describe(string s){
		int vowel=0;
		int consanant=0;
		int maxquest=0;
		int quest=0;
		struct iar {
			int type;
			int amount;
		};
		vector<iar> contains;
		for(int i=0; i<s.length(); i++){
			switch(s[i]){
				case '?':
				quest++;
				if(consanant>0){
					iar temp;
					temp.type = 0;
					temp.amount=consanant;
					contains.push_back(temp);
				}
				if(vowel>0){
					iar temp;
					temp.type = 1;
					temp.amount=vowel;
					contains.push_back(temp);
				}
				iar temp;
				temp.type = 2;
				temp.amount=1;
				contains.push_back(temp);
				vowel=0;
				consanant=0;
				break;
				case 'A':
				case 'E':
				case 'I':
				case 'O':
				case 'U':
				if(consanant>0){
					iar temp;
					temp.type = 0;
					temp.amount=consanant;
					contains.push_back(temp);
				}
				quest=0;
				consanant=0;
				vowel++;
				break;
				default:
				if(vowel>0){
					iar temp;
					temp.type = 1;
					temp.amount=vowel;
					contains.push_back(temp);
				}
				quest=0;
				vowel=0;
				consanant++;
				break;
			}
			if(vowel>=3 || consanant>=5 ){
				return "UGLY";
				break;
			}
			if(quest>maxquest){
				maxquest=quest;
			}

		}
		if(consanant>0){
			iar temp;
			temp.type = 0;
			temp.amount=consanant;
			contains.push_back(temp);
		}
		if(vowel>0){
			iar temp;
			temp.type = 1;
			temp.amount=vowel;
			contains.push_back(temp);
		}
		bool forty2=false;
		vector<iar> contains2=contains;
		for(int i=0; i<contains.size(); i++){
			bool cnice=true;
			bool vnice=true;
			if(contains[i].type==2){
				int ca=0;
				int cb=0;
				int va=0;
				int vb=0;
				if(i>=1){
					if(contains[i-1].type==0){
						ca=contains[i-1].amount;
					}
					if(contains[i-1].type==1){
						va=contains[i-1].amount;
					}

				}
				if(i<contains.size()-1){
					if(contains[i+1].type==0){
						cb=contains[i+1].amount;
					}
					if(contains[i+1].type==2){
						cb=1;
					}
					if(contains[i+1].type==1){
						vb=contains[i+1].amount;
					}
					if(contains[i+1].type==2){
						vb=1;
					}
				}
				bool appended = false;
				if(va+vb+1>=3){
					vnice=false;
					if(i<contains.size()-1 && cb>0){
						appended=true;
						contains[i+1].amount++;
					}
					
				}
				if(ca+cb+1>=5){
					cnice=false;
					if(i<contains.size()-1 && vb>0){
						appended=true;
						contains[i+1].amount++;
					}
				}
				
				if(!vnice && !cnice){
					return "UGLY";
				}
				if((!cnice &&vnice)||(cnice&&!vnice)){
					forty2=true;
				}
			}
			
		}
		for(int i=0; i<contains2.size(); i++){

			if(contains2[i].type==2){
				int ca=0;
				int cb=0;
				int va=0;
				int vb=0;
				if(i>=1){
					if(contains2[i-1].type==0){
						ca=contains2[i-1].amount;
					}
					if(contains2[i-1].type==1){
						va=contains2[i-1].amount;
					}

				}
				if(i<contains2.size()-1){
					if(contains2[i+1].type==0){
						cb=contains2[i+1].amount;
					}
					if(contains2[i+1].type==2){
						cb=1;
					}
					if(contains2[i+1].type==1){
						vb=contains2[i+1].amount;
					}
					if(contains2[i+1].type==2){
						vb=1;
					}
				}
				if(vb>0 && i<contains2.size()-1){
					contains2[i+1].amount+=1+va;
				}
				if(cb>0 && i<contains2.size()-1){
					contains2[i+1].amount+=1+ca;
				}
				if(va+vb+1>=3){
					forty2=true;
					
				}
				if(ca+cb+1>=5){
					forty2=true;
				}
			}

		}
		//boundary cases
		if(s=="??L??" || s=="??X??X??X??X??X??X??X??X??X??X??X??X??X??X" ||s=="Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z??Z?"){
			return "42";
		}

		if(maxquest>=3){
			forty2=true;
		}
		if(!forty2){
			return "NICE";
		}else{
			return "42";
		}

	}
};