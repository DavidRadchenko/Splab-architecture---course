extern char state[];
extern int WorldWidth;
extern int WorldLength;


int aliveNeighbers(int x,int y) {
        int ans = 0;
        int x1, y1 ;

         x1=x-1;
         if(x1<0) x1=x1+WorldLength;
         y1=y-1; 
         if(y1<0) y1=y1+WorldWidth;
         
         /* one line up */ 
         if(state[(x1)%(WorldLength)*WorldWidth + (y1)%(WorldWidth)] >= 1) 
             ans++;

         if(state[(x1)%(WorldLength)*WorldWidth + (y)%(WorldWidth)] >= 1) 
             ans++;
         
         if(state[(x1)%(WorldLength)*WorldWidth + (y+1)%(WorldWidth)]  >= 1) 
             ans++;
         /* one line down */ 
         if(state[(x+1)%(WorldLength)*WorldWidth + (y1)%(WorldWidth)]  >= 1) 
             ans++;
        
         if(state[(x+1)%(WorldLength)*WorldWidth + (y)%(WorldWidth)] >= 1) 
             ans++;
         
         if(state[(x+1)%(WorldLength)*WorldWidth + (y+1)%(WorldWidth)]  >= 1) 
             ans++;
         
         
         /* two cells on the both sides */ 
          if(state[(x)%(WorldLength)*WorldWidth + (y1)%(WorldWidth)]  >= 1) 
             ans++;
         
         if(state[(x)%(WorldLength)*WorldWidth + (y+1)%(WorldWidth)]  >= 1) 
             ans++;
         
         
        
        return ans ; 
}

extern int computeState(int x, int y) {


        int ans = aliveNeighbers(x, y);
        if(state[x*WorldWidth + y] >=1) {
                if(ans == 2 || ans == 3)
                        return 1;
                else
                        return 0;
        }
        else {
                if(ans == 3)
                        return 1;
                else
                        return 0;
        }
}

