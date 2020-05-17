void Ack(int m, int n, int* result){
    if(m == 0){ 
        *result = n+1;
        return;
    }
    else if( n== 0){ 
        Ack(m-1,1,result);
    }
    else{ 
        int temp = 0;
        Ack(m,n-1,&temp);
        Ack(m-1,temp,result);
    }
}


int main() {
    int v = 0;
	Ack( 2, 2, &v);
    cout << v << endl;
	return 0;
}