#include <stdio.h>
#include <stdlib.h>
#include "mpi/mpi.h"
#include <math.h>
unsigned numdigit(const unsigned n)
{
    if(n<10) return 1;
    return 1+ numdigit(n/10);
}
int max(int arr[],int size)
{
    int maximum=-999999;
    for(int i=1; i<=size; i++)
    {
        if(maximum<arr[i])
        {
            maximum=arr[i];
        }
    }
    return maximum;
}
int maxindex(int arr[],int size)
{
    int maximum=-99999;
    int index;
    for(int i=1; i<=size; i++)
    {
        if(maximum<arr[i])
        {
            maximum=arr[i];
            index=i;
        }
    }
    return index;
}
int main(int argc,char * argv[])
{
    int NumberOfCandidates;
    int NumberOfVoters;
    int size;
    int line=0;
    int myline=0;
    int hey, sum;
    MPI_Init(&argc,&argv);
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(myrank==0)
    {
        FILE *fs=fopen("input.txt","w");
        if(fs==NULL)
        {
            printf("Error Opening file\n");
            exit(1);
        }
        printf("Please Enter Number Of Candidates : ");
        scanf("%d",&NumberOfCandidates);
        fprintf(fs,"%d\n",NumberOfCandidates);
        printf("Please Enter Number Of Voters : ");
        scanf("%d",&NumberOfVoters);
        fprintf(fs,"%d\n",NumberOfVoters);
        for(int i=0;i<NumberOfVoters;i++)
        {
            for(int j=0;j<NumberOfCandidates;j++)
            {
                printf("Please Enter preference Number %d",j+1);
                int pref;
                scanf("%d",&pref);
                fprintf(fs,"%d",pref);
                if(j+1<NumberOfCandidates){fprintf(fs," ");}
            }
            fprintf(fs,"\n");
        }
        line=2+NumberOfVoters;
        fclose(fs);
    }
    MPI_Bcast(&NumberOfCandidates,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&NumberOfVoters,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&line,1,MPI_INT,0,MPI_COMM_WORLD);
    int counter[NumberOfCandidates];
    int arr[NumberOfCandidates][NumberOfVoters];

    int linesize=NumberOfCandidates+NumberOfCandidates-1;
    int x=0;
    if(NumberOfVoters%size==0)
    {
        int y=0;
        char test[NumberOfCandidates*2]/* *2 cause of " "*/;
        FILE *fp=fopen("input.txt","r");
        while(y<((line-2)/size))
        {
            int seeker=numdigit(NumberOfCandidates)+numdigit(NumberOfVoters)+2;
            seeker+=myline*(linesize+1)+myrank*(linesize+1)*(NumberOfVoters/size);
            myline++;
            fseek(fp,seeker,SEEK_SET);
            fgets(test,NumberOfCandidates*2,fp);
            for(int i=0; i<NumberOfCandidates*2-1; i++)
            {
                if((test[i]>='0'||test[i]<=(char)(NumberOfCandidates)))
                {
                    counter[atoi(&test[i])]=-1*i;
                }
            }
                int xxx;
                for(xxx=1; xxx<=NumberOfCandidates; xxx++)
                {
                    arr[xxx][myline+myrank*(NumberOfVoters/size)-1]=counter[xxx];
                }
            y++;
        }
    }
    else
    {
        int y=0;
        char test[NumberOfCandidates*2]/* *2 cause of " "*/;
        FILE *fp=fopen("input.txt","r");
        while(y<((line-2)/size))
        {
            int seeker=numdigit(NumberOfCandidates)+numdigit(NumberOfVoters)+2;
            seeker+=myline*(linesize+1)+myrank*(linesize+1)*(NumberOfVoters/size);
            myline++;
            fseek(fp,seeker,SEEK_SET);
            fgets(test,NumberOfCandidates*2,fp);
            for(int i=0; i<NumberOfCandidates*2-1; i++)
            {
                if((test[i]>='0'||test[i]<(char)(NumberOfCandidates)))
                {
                    counter[atoi(&test[i])]=-1*i;
                }
            }
                int xxx;
                for(xxx=1; xxx<=NumberOfCandidates; xxx++)
                {
                    arr[xxx][myline+myrank*(NumberOfVoters/size)-1]=counter[xxx];
                }
            y++;
        }
        if(myrank==0)
        {
            int y=0;
            char test[NumberOfCandidates*2]/* *2 cause of " "*/;
            FILE *fp=fopen("input.txt","r");
        while(y<((line-2)% size))
        {
            int seeker=numdigit(NumberOfCandidates)+numdigit(NumberOfVoters)+2;
            seeker+=myline*(linesize+1)+myline*(linesize+1)*(size-1);
            myline++;
            fseek(fp,seeker,SEEK_SET);
            fgets(test,NumberOfCandidates*2,fp);
            for(int i=0; i<NumberOfCandidates*2-1; i++)
            {
                if((test[i]>='0'||test[i]<(char)(NumberOfCandidates)))
                {
                    counter[atoi(&test[i])]=-1*i;
                }
            }
                int xxx;
                for(xxx=1; xxx<=NumberOfCandidates; xxx++)
                {
                    arr[xxx][myline+(size-1)*((line-2)/size)-1]=counter[xxx];
                }
            y++;
        }
    }
    }
    int res[NumberOfCandidates+1][NumberOfVoters];
    for(int i=1; i<=NumberOfCandidates; i++)
    {
        for(int j=0; j<NumberOfVoters; j++)
        {
            if(arr[i][j]>1000||arr[i][j]<-100000)
            {
                arr[i][j]=0;
            }
            MPI_Reduce(&arr[i][j],&res[i][j],1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
        }
    }
    if(myrank==0)
    {
        int counter1[NumberOfCandidates+1];
        int counter2[NumberOfCandidates+1];
        for(int i=1; i<=NumberOfCandidates; i++)
        {
            counter1[i]=0;
            counter2[i]=0;
        }
        for(int n=1; n<=NumberOfCandidates; n++)
        {
            for(int m=0; m<NumberOfVoters; m++)
            {
                counter2[n]+=res[n][m];
                if(res[n][m]==0)
                {
                    counter1[n]++;
                }
            }
        }
        int numberofwinners=0;
        for(int i=1; i<=NumberOfCandidates; i++)
        {
            if(counter1[i]==max(counter1,NumberOfCandidates))
            {
                numberofwinners++;
            }
        }
        if(numberofwinners!=1)
        {
            int winner=maxindex(counter2,NumberOfCandidates);
            printf("Winner is Number %d in round 2\n",winner);
        }
        else
        {
            int winner=maxindex(counter1,NumberOfCandidates);
            printf("Winner is Number %d in round 1\n",winner);
        }
    }
    MPI_Finalize();
    return 0;
}
