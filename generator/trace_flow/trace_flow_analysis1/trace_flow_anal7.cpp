//one lever for auto distribution
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
using namespace std;
#define NUM 80
#define TILE 48  
//#define OCCUP 12
#define TOTAL_COUNT 64


struct test {
    int s;
    int d;
    int value;
};
bool sort_value(const test &x, const test &y);
int manhattan_dis(int s,int d);
vector < test > So;
vector < vector < int > > C;


int main(int argc, char** argv)
{
    int iArg=1;
    char name[100];
    if(!strcmp(argv[iArg],"-n")){
        strcpy(name,argv[iArg+1]);
    }
    cout<<name<<endl;

	int s_x,s_y,d_x,d_y,pack_val,s_id,d_id;
	long int cycle;
    char init_name[100];
	fstream fin;
    strcpy(init_name,"../flow_");
    strcat(init_name,name);
    strcat(init_name,"_org.log");
	C.resize(NUM);
	for (int i=0;i<NUM;i++)
	{
		C[i].resize(NUM);
	}
//	freopen("in.txt","r",stdin);
	fin.open(init_name);
 //   fin.open("in.log");
	printf("just test\n");
//	fin>>s_x>>s_y>>d_x>>d_y>>pack_val;
	while(!fin.eof())
	{
		fin>>s_x>>s_y>>d_x>>d_y>>cycle>>pack_val;
 //       printf("%d %d %d %d %d %d\n",s_x,s_y,d_x,d_y,cycle,pack_val);
		s_id=s_x*8+s_y;
		d_id=d_x*8+d_y;
	//	printf("%d %d\n",s_id,d_id);
		C[s_id][d_id]=C[s_id][d_id]+pack_val;
	//	printf("%d\n",C[s_id][d_id]);
	}
	fin.close();
	cout<<"trace flow calculate"<<endl;

	ofstream TF;
    char TF_name[100];
    strcpy(TF_name,"./output7/flow_");
    strcat(TF_name,name);
    strcat(TF_name,"_TF.txt");
	TF.open(TF_name);
	for (int i=0;i<NUM;i++)
	{
		for (int j=0;j<NUM;j++)
		{
			TF<<C[i][j]<<"   ";
		}
		TF<<endl;
	}
	TF.close();
	cout<<"trace flow output finished"<<endl;

    ofstream Graph;
    char Graph_name[100];
    strcpy(Graph_name,"./output7/flow_");
    strcat(Graph_name,name);
    strcat(Graph_name,"_Graph.txt");
    Graph.open(Graph_name);
    for(int i=0;i<NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            if(C[i][j]!=0)
            {
                Graph<<i<<"  "<<j<<"  "<<C[i][j];
                Graph<<endl;
            }

        }
    }
    Graph.close();
    cout<<"graph output finished"<<endl;



   ofstream Graph_choose;
   ofstream Gephi_choose;
   char Graph_choose_name[100];
   char Gephi_choose_name[100];
   strcpy(Graph_choose_name,"./output7/flow_");
   strcat(Graph_choose_name,name);
   strcat(Graph_choose_name,"_Graph_choose.txt");
   strcpy(Gephi_choose_name,"./output7/flow_");
   strcat(Gephi_choose_name,name);
   strcat(Gephi_choose_name,"_Gephi_choose.csv");
    Graph_choose.open(Graph_choose_name);
    Gephi_choose.open(Gephi_choose_name);
    Gephi_choose<<"Source,target";
    Gephi_choose<<endl;
    int sum_c=0;
  //  So.resize(500);
    for(int i=8;i<TILE+8;i++)
    {
        for(int j=8;j<TILE+8;j++)
        {
            if(C[i][j]!=0)
            {
                Graph_choose<<i<<"  "<<j<<"  "<<C[i][j];
                Graph_choose<<endl;
                Gephi_choose<<i<<","<<j;
                Gephi_choose<<endl;
                sum_c++;
 //               comm_cost1+=manhattan_dis(Location1[i],Location1[j])*C[i][j];
  //              comm_cost2+=manhattan_dis(Location2[i],Location2[j])*C[i][j];
            }

        }
    }
    Graph_choose.close();
    Gephi_choose.close();
    cout<<"graph_choose output finished"<<endl;
    cout<<"gephi_choose output finished"<<endl;
    cout<<"sum communication num:"<<sum_c<<endl;
//    cout<<"communication cost of scheme1 of 8*8:"<<comm_cost1<<endl;

    So.resize(sum_c);
    ofstream Graph_sort;
    char Graph_sort_name[100];
    strcpy(Graph_sort_name,"./output7/flow_");
    strcat(Graph_sort_name,name);
    strcat(Graph_sort_name,"_Graph_sort.csv");
    Graph_sort.open(Graph_sort_name);
    int k=0;
    for(int i=8;i<TILE+8;i++)
    {
        for(int j=8;j<TILE+8;j++)
        {
            if(C[i][j]!=0)
            {
                So[k].s=i;
                So[k].d=j;
                So[k].value=C[i][j];
                k++;
            }

        }
    }
    sort(So.begin(),So.end(),sort_value);
    for(int i=0;i<sum_c;i++)
    {
        Graph_sort<<So[i].s<<","<<So[i].d<<","<<So[i].value;
        Graph_sort<<endl;
    }
    Graph_sort.close();
    cout<<"graph_sort output finished"<<endl;

    long int comm_cost1=0,comm_cost2=0;
    int sort_count=0,core_count=0,Data_count=0;
    vector <int> Location1;
    vector <int> Location2;
    Location1.resize(TILE+8);
    Location2.resize(TILE+8);
    int model1[TILE]={0,1,6,7,8,9,14,15,16,17,22,23,24,25,30,31,32,33,38,39};
    int model2[TILE]={26,27,28,29,34,35,36,37,42,43,44,45,50,51,52,53,58,59,60,61};
    int flag[8+TILE];
    for(int i=8;i<8+TILE;i++)
        flag[i]=0;
    while(core_count<TILE){
        if(flag[So[sort_count].s]==0){
            flag[So[sort_count].s]=1;
            Location2[So[sort_count].s]=model2[Data_count];
            Location1[So[sort_count].s]=model1[Data_count];
            Data_count++;
            core_count++;
        }
         if(flag[So[sort_count].d]==0){
            flag[So[sort_count].d]=1;
            Location2[So[sort_count].d]=model2[Data_count];
            Location1[So[sort_count].d]=model1[Data_count];
            Data_count++;
            core_count++;
        }
        sort_count++;
    }
    for(int i=0;i<TILE+8;i++){
        cout<<Location1[i]<<"  ";
    }
    cout<<endl;
    for(int i=0;i<TILE+8;i++){
        cout<<Location2[i]<<"  ";
    }
    cout<<endl;
    for(int i=8;i<TILE+8;i++){
        for(int j=8;j<TILE+8;j++){
            if(C[i][j]!=0){
                comm_cost1+=manhattan_dis(Location1[i],Location1[j])*C[i][j];
                comm_cost2+=manhattan_dis(Location2[i],Location2[j])*C[i][j];
            }
        }
    }
    float prop=(float)comm_cost1/comm_cost2;
    cout<<comm_cost1<<"  "<<comm_cost2<<"  "<<prop<<endl;

    cout<<"communication cost finished "<<endl;




//for gephi input file
    ofstream Gephi_weight;
    char Gephi_weight_name[100];
    strcpy(Gephi_weight_name,"./output7/flow_");
    strcat(Gephi_weight_name,name);
    strcat(Gephi_weight_name,"_Gephi_weight.csv");
    Gephi_weight.open(Gephi_weight_name);
    Gephi_weight<<"Source,Target";
    Gephi_weight<<endl;
    int lever=0;
    for(int i=0;i<NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            if(C[i][j]>=100)
            {
                lever=C[i][j]/100;
                if(lever>10)
                    lever=10;
                for(int k=0;k<lever;k++)
                {
                    Gephi_weight<<i<<","<<j;
                    Gephi_weight<<endl;
                }

            }

        }
    }
    Gephi_weight.close();
    cout<<"Gephi_weight output finished"<<endl;

    ofstream Gephi;
    char Gephi_name[100];
    strcpy(Gephi_name,"./output7/flow_");
    strcat(Gephi_name,name);
    strcat(Gephi_name,"_Gephi.csv");
    Gephi.open(Gephi_name);
    Gephi<<"Source,Target";
    Gephi<<endl;
    for(int i=0;i<NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            if(C[i][j]!=0)
            {
                Gephi<<i<<","<<j;
                Gephi<<endl;
            }

        }
    }
    Gephi.close();
    cout<<"Gephi output finished"<<endl;


	return 0;
}

bool sort_value(const test &x, const test &y)
{
    return x.value>y.value;
}
int manhattan_dis(int s,int d)
{
    int XDIM=(int)sqrt(TOTAL_COUNT);
    int x_axis_s,x_axis_d,y_axis_s,y_axis_d;
    int dis;
    x_axis_s=s%XDIM;
    y_axis_s=s/XDIM;
    x_axis_d=d%XDIM;
    y_axis_d=d/XDIM;
    dis=abs(x_axis_s-x_axis_d)+abs(y_axis_s-y_axis_d);
    return dis;
}
