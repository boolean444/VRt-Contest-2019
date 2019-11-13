#include <bits/stdc++.h>
using namespace std;
const double pi = acos(-1.);
const long long mod = 1000000007;

//printf("%.12lf\n",mid);

inline void prep()
{
    cin.tie(0);
    cin.sync_with_stdio(0);
};


int dists[2000][2000];
vector<pair<pair<int, int>, int> > cityavgdist;
bool memoo[2001][802];
int memo[2001][802];
// for backtrace, 
int n;
int gain[2000];
bool taken[2000];
bool realtaken[2000];
vector<int> dur;
vector<int> ppl;
int currlim, ret;
int upuntil;
bool now = false;
int func(int ind, int r, int lim, int last){
  //cout << ind << " "<< r << endl;
 
  if (r > currlim){
    return -1000000000;
  }
  if (ind == upuntil){
      if (r+dists[min(ret, last)][max(ret, last)] > currlim){
          return -1000000000;
      }else{
          return -dists[min(ret, last)][max(ret, last)];
      }
      
  }
  if (ind >= n){
      if (r+dists[min(ret, last)][max(ret, last)] > currlim){
          return -1000000000;
      }else{
          return -dists[min(ret, last)][max(ret, last)];
      }
  }
  if (memoo[ind][r]){
    return memo[ind][r];
  }
  
  int curl = cityavgdist[ind].first.first;
  int curr = cityavgdist[ind].first.second;
  int actualind = cityavgdist[ind].second;
  
  int res = -1000000000;
  if (ppl[actualind] <= lim  && !taken[actualind] && !realtaken[actualind]){
    int addon = 0;
    int gainn = 0;
   
    if (actualind != 0){ 
        int ll = min(last, actualind);
        int rr = max(last, actualind);
        addon = dists[ll][rr];
       
        gainn = gain[actualind]-addon;
    }
   
    int nextr = max(addon+r, curl)+dur[actualind];
        
    if (nextr > curr+dur[actualind] || nextr > currlim){
        
        res = max(res, func(ind+1,r, lim, last));
    }else{
        
        
        int ll = min(ret, actualind);
        int rr = max(ret, actualind);
        if (nextr+dists[ll][rr] <= currlim){
            res = max(res, gainn+func(ind+1,nextr, lim, actualind));
            res = max(res, gainn-dists[ll][rr]);
        }
        
        res = max(res, func(ind+1,r, lim, last));
    }
    memoo[ind][r] = true;
    memo[ind][r] = res;
    return res;
  }else{
    res = max(res, func(ind+1,r, lim, last));
    memoo[ind][r] = true;
    memo[ind][r] = res;
    //cout << res << endl;
    return res;
  }
  
}
vector<int> path;
void backtrace(int ind, int r, int lim, int last){
    if (ind >= n || r > currlim || ind == upuntil){
        return;
    }
  
    int curl = cityavgdist[ind].first.first;
    int curr = cityavgdist[ind].first.second;
    int actualind = cityavgdist[ind].second;
    //cout << ppl[actualind] << " "<< lim << endl;
    
    if (ppl[actualind] <= lim && !taken[actualind] && !realtaken[actualind] ){
        int addon = 0;
        int gainn = 0;
        if (actualind != 0){
            int ll = min(last, actualind);
            int rr = max(last, actualind);
            addon = dists[ll][rr];
            gainn = gain[actualind]-addon;
        }
        int nextr = max(addon+r, curl)+dur[actualind];
        if (nextr > curr+dur[actualind] || nextr > currlim){
            backtrace(ind+1, r, lim, last);
        }else{
            int vall = gainn+memo[ind+1][max(addon+r, curl)+dur[actualind]];
            int ll = min(ret, actualind);
            int rr = max(ret, actualind);
            int currdist = dists[ll][rr];
            //cout << vall << " " << memo[ind+1][r] << endl;
            if (nextr+dists[ll][rr] <= currlim){
                if (vall > memo[ind+1][r] && vall > gainn-currdist){
                    path.push_back(actualind);
                    backtrace(ind+1, max(addon+r, curl)+dur[actualind], lim, actualind);
                }else if (memo[ind+1][r] < gainn-currdist){
                    path.push_back(actualind);
                }else{
                    backtrace(ind+1, r, lim, last);
                }
            }else{
                
                backtrace(ind+1, r, lim, last);
            }
            
        }
        
    }else{
        backtrace(ind+1, r, lim, last);
    }
    
}
int main(){
  prep();
  double start = clock();
  cin >> n;
  vector<pair<int, int> > xy;
  
  vector<pair<int, int> > el;
  int total = 0;
  int minppl = 1000000;
  int mapping[n];
  for (int i=0; i<n; i++){
    
    int x, y, d, p, e, l;
    cin >> x >> y >> d >> p >> e >> l;
 
    xy.emplace_back(x, y);
    dur.push_back(d);
    ppl.push_back(p);
    if (i != 0){
        minppl = min(p, minppl);
    }
    el.emplace_back(e, l-d);
    total += p;
    cityavgdist.push_back({el[i], i});
  }
  
  for (int i=0; i<n; i++){
    gain[i] = dur[i]*ppl[i]*(ppl[i]+4);
  }
  
  //cout << endl;
  for (int i=0; i<n; i++){
    for (int j=i+1; j<n; j++){
      
      dists[i][j] = abs(xy[i].first-xy[j].first)+abs(xy[i].second-xy[j].second);
    }
  }
  sort(cityavgdist.begin(), cityavgdist.end());
  for (int i=0; i<n; i++){
      mapping[cityavgdist[i].second] = i;
  }
  int final = 0;
  int lines = 0;
  // set # of workers
  // give first worker subset of jobs (if going to job w/ n ppl, multiply cost to average time * ppl to see if job is good). if job needs more than 1 person assign second, third, fourth... workers to it
  // give second worker subset of jobs but only ones with 1 person, and if not 1 person, jobs that first worker has, and for each special job, make sure that before the special job, the worker is already on their way there. Do this by seeing if you can do any jobs before the first special job that aren't already taken, and take the most profitable.
  bool didbefore = false;
  int g = 0;
  // the higher n is, do g more
  
  int orign = n-500;
  int linear = 52-(750-abs(750-orign))/62.5;
  int upper;
  if (750-orign > 0){
      int far = abs(750-orign);
      upper = 25+far/47;
  }else if (750-orign < 0){
      int far = abs(750-orign);
      upper = 25-far/54;
  }else{
      upper = 25;
  }
 
  int currtop = linear-upper;
  while (true){
      int maxx = -1000000000;
      int many = 0;
      vector<pair<int, int> > realgot[60];
     
      for (int i=minppl; i<=upper; i++){ 
        int tempppl[2000];
        for (int j=0; j<n; j++){
            tempppl[j] = ppl[j];
        }
        for (int j=0; j<n; j++){
            taken[j] = false;
        }
        int neww[60] = {0};
        int lastcit[60] = {0};
        vector<pair<int, int> > got[60];
        long long minires = -240*i;
        int minus = -1;
        
        for (int x=0; x<i; x++){
            if(clock()-start > 14.9 * CLOCKS_PER_SEC) exit(0);
            minus++;
            for (int j=0; j<2001; j++){
                for (int k=0; k<802; k++){
                    memoo[j][k] = false;
                    memo[j][k] = 0;
                }
            }
            
            if (x == 0){
                
                ret = 0;
                currlim = 1000;
                upuntil = n;
                
                int calc = func(1, 0, i-minus, 0);
                path.clear();
                backtrace(1, 0, i-minus, 0);
                minires += calc;  
                for (int j=0; j<path.size(); j++){
                    int currind = path[j];
                    
                    tempppl[currind]--;
                    if (!neww[0]){
                      got[0].push_back({0, el[currind].first-dists[0][currind]});
                      got[0].push_back({currind, el[currind].first});
                      neww[0] = el[currind].first+dur[currind];
                      lastcit[0] = currind;
                  }else{
                      int l = min(lastcit[0], currind);
                      int r = max(lastcit[0], currind);
                      int vall = max(neww[0]+dists[l][r], el[currind].first);
                      got[0].push_back({currind, vall});
                      neww[0] = vall+dur[currind];
                      lastcit[0] = currind;
                  }
                }
                
            }else{
                vector<pair<int, int> > lastlst = got[x-1];
                
                // make sure job is more or equal to x people
                for (int j=1; j<lastlst.size(); j++){
                    int actualind = lastlst[j].first;
                    
                    int dursofar = lastlst[j].second;
                    if (tempppl[actualind] == 0){
                        //cout << x << " " << j << endl;
                        taken[actualind] = true;
                        continue;
                    }
                   
                    currlim = dursofar;
                    
                    ret = actualind;
                    upuntil = mapping[actualind];
                    
                    int calc = func(mapping[lastcit[x]]+1, neww[x], i-minus, lastcit[x]);
                    path.clear();
                    backtrace(mapping[lastcit[x]]+1, neww[x], i-minus, lastcit[x]);
                   
                    minires += calc;
                    
                    for (int k=0; k<path.size(); k++){
                         
                   
                        int currind = path[k];
                        tempppl[currind]--;
                        if (!neww[x]){
                          got[x].push_back({0, el[currind].first-dists[0][currind]});
                          got[x].push_back({currind, el[currind].first});
                          neww[x] = el[currind].first+dur[currind];
                          lastcit[x] = currind;
                      }else{
                          int l = min(lastcit[x], currind);
                          int r = max(lastcit[x], currind);
                          int vall = max(neww[x]+dists[l][r], el[currind].first);
                          
                          got[x].push_back({currind, vall});
                          neww[x] = vall+dur[currind];
                          lastcit[x] = currind;
                      }
                    }
                    
                    if (path.size() == 0 && !neww[x]){
                        got[x].push_back({0, dursofar-dists[0][actualind]});
                        got[x].push_back({actualind, dursofar});
                        neww[x] = dursofar+dur[actualind];
                    }else{
                        got[x].push_back({actualind, dursofar});
                        neww[x] = dursofar+dur[actualind];
                    }
                    tempppl[actualind]--;
                    lastcit[x] = actualind;
                   
                }
                if(clock()-start > 14.9 * CLOCKS_PER_SEC) exit(0);
                ret = 0;
                currlim = 1000;
                upuntil = n;
                int calc = func(mapping[lastcit[x]]+1, neww[x], i-minus, lastcit[x]);
                minires += calc;
                //cout << calc << endl;
                path.clear();
                backtrace(mapping[lastcit[x]]+1, neww[x], i-minus, lastcit[x]);
                for (int k=0; k<path.size(); k++){
                
                    int currind = path[k];
                   
                    tempppl[currind]--;
                    if (!neww[x]){
                     
                      got[x].push_back({0, el[currind].first-dists[0][currind]});
                      got[x].push_back({currind, el[currind].first});
                      neww[x] = el[currind].first+dur[currind];
                      lastcit[x] = currind;
                  }else{
                      int l = min(lastcit[x], currind);
                      int r = max(lastcit[x], currind);
                      int vall = max(neww[x]+dists[l][r], el[currind].first);
                      got[x].push_back({currind, vall});
                      neww[x] = vall+dur[currind];
                      lastcit[x] = currind;
                  }
                }
            }
           // cout << minires << " " <<x <<endl;
        }
        
        if (minires > maxx){
            many = i;
            maxx = minires;
            for (int j=0; j<i; j++){
                realgot[j] = got[j];
            }
        }
      }
      if(clock()-start > 14.9 * CLOCKS_PER_SEC) exit(0);
      if (maxx > 0 || (!didbefore && maxx <= 0)){
          //cout << many << endl;
         
          for (int i=0; i<many; i++){
                lines++;
              int last;
              int lastdist;
              if (lines!= 282 && lines!= 281){
                  //continue;
              }
              for (int j=0; j<realgot[i].size(); j++){
                  if (j < 7){
                      //continue;
                  }
                  
                  pair<int, int> job = realgot[i][j];
                  realtaken[job.first] = true;
                  if (j == 0){
                    printf("start %d %d\n", job.second, job.first+1);
                   
                  }else{
                    printf("arrive %d %d\n", job.second, job.first+1);
                    printf("work %d %d %d\n", job.second, job.second+dur[job.first], job.first+1);
                    
                    lastdist = job.second+dur[job.first];
                  }
                  last = job.first+1;
                  
              }
              printf("arrive %d 1\n", dists[0][last-1]+lastdist);
              printf("end\n");
          }
          final += maxx;
          bool istrue = false;
          for (int i=0; i<n; i++){
              if (!realtaken[i]){
                  istrue = true;
              }
          }
          if (!istrue){
              break;
          }
          if (!didbefore && maxx <= 0){
              break;
          }
          didbefore = true;
           
          if(clock()-start > 14.9 * CLOCKS_PER_SEC) exit(0);
          
      }else{
          break;
      }
      
      
  }
  //cout << g << endl;
  //cout << final << endl;
  // later sort between first start and size of time span
  //cout << dists[45][555] << endl;

  
  
  return 0;
}
/* 5
5 15 0 0 0 0
2 13 60 2 200 273
15 12 60 1 200 273
10 12 60 1 200 600
39 21 9 4 671 757*/
// in dp problems think: up until here, intervals, or "what if there's 1, 2, 3..."
// in math problems write out equations
// also in math problems to change multiplication/division to addition/subtraction use logs
// if printint pow function, put an (int) or (long long) next to it
// if checked everything check for bugs (like sorting wrong way or not putting a break in a loop)
// if n is 10^7 try brute force if the operations are simple
// if can't do dp do brute force/greedy
// to save time don't copy maps/vectors into variables if it already exists
// only use long long if need to
