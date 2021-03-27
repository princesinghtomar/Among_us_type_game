#include<bits/stdc++.h>
int *vertices;
int main(){
	int i=0 ;
	vertices = new int[7];
	for(int j=0;j<7;j++){
		vertices[i++] = 5;
		std::cout<< "Vertices[" << i-1 << "] : " << vertices[i-1] << std::endl;
	}
	return 0;
}
