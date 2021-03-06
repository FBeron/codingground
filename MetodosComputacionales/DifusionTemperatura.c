#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

#define DX 0.1
#define DT 0.1
#define L 102
#define A 1
#define B 101
#define V  0.0001

void imprimir(FILE *in,float data[L][L]);
void guardar_placa(float data[L][L], float t, int caso, int condicion);
void guardar_tpromedio(float Tp, float t, int caso, int condicion, bool first);
void copy(float data_1[L][L], float data_2[L][L]);
void init_temp(float data[L][L]);
void solve_dt(float data[L][L], float alpha, int caso, int condicion);
float t_prom(float data[L][L]);
void condiciones(float data[L][L], int condicion);
void nombreArchivo(char filename[100],float t, int caso, int condicion);

int main(){
    float alpha = V*DT/pow(DX,2);
    float temp[L][L];
    int caso;
    int condicion;
    int t;
    float Tp;
    
    for(caso=1; caso<=2;caso++){
        for(condicion=1; condicion<=3; condicion++){
            bool first = true;
            init_temp(temp);
            t = 0;
            guardar_placa(temp,t, caso, condicion);
            Tp = t_prom(temp);
            guardar_tpromedio(Tp, t,caso, condicion,first);
            first = false;
            t = DT;
            while(t <= 2500.0){
                solve_dt(temp, alpha, caso, condicion);
                guardar_placa(temp,t, caso, condicion);
                Tp = t_prom(temp);
                guardar_tpromedio(Tp, t,caso, condicion,first);
                t = t + DT;
            }
        }
    }
    return 0;
}

void imprimir(FILE *in,float data[L][L]){
    int i,j;
    for(i=A; i<B;i++){
        for(j=A; j<B;j++){
            fprintf(in, "%f\t", data[i][j]);
        }
        fprintf(in, "\n");
    }
}

void guardar_placa(float data[L][L], float t, int caso, int condicion){
    FILE *in;
    char filename[100]="TPlaca";
    nombreArchivo(filename, t, caso, condicion);
    in = fopen(filename,"w");
    imprimir(in, data);
    fclose(in);
}

void guardar_tpromedio(float Tp, float t, int caso, int condicion, bool first){
    FILE *in2;
    char filename[100]="Tpromedio";
    nombreArchivo(filename, t, caso, condicion);
    if(first){
        in2 = fopen(filename,"w");
        fprintf(in2, "%f\t%f\n", t, Tp);
        fclose(in2);
    }else{
        in2 = fopen(filename,"a");
        fprintf(in2, "%f\t%f\n", t, Tp);
        fclose(in2);
    }
    
}

void copy(float data_1[L][L], float data_2[L][L]){
    int i,j;
    for(i=A;i<B;i++){
        for(j=A;j<B;j++){
            data_1[i][j] = data_2[i][j];
        }   
    }
}

void init_temp(float data[L][L]){
    int i, j;
    for(i=0; i<L; i++){
        for(j=0; j<L; j++){
            if((i>=A+20) && (i<A+40) && (j>=A+45) && (j<A+55)){
	            data[i][j] = 100.0;
            }else{
	            data[i][j] = 50.0;
            }
        }
    }
}

void solve_dt(float data[L][L], float alpha, int caso, int condicion){
    float data_new[L][L];
    
    condiciones(data, condicion);
    
    int i,j;
    for(i=A; i<B;i++){
        for(j=A;j<B;j++){
            data_new[i][j] = data[i][j] +  alpha*(data[i+1][j] + data[i-1][j] + data[i][j+1] + data[i][j-1]- 4*data[i][j]);
        }
    }
    copy(data, data_new);
}

float t_prom(float data[L][L]){
    int i,j;
    float sum, num, avg;

    sum = 0.0;
    num = 0.0;

    for(i=A; i<B;i++){
        for(j=A; j<B;j++){
            sum = sum + data[i][j];
            num = num + 1.0;
        }
    }

    avg = sum/num;
    return avg;
}

void condiciones(float data[L][L], int condicion){
    int i;
    if (condicion ==1){
        for(i=A;i<B;i++){
            data[i][0] = data[i][1];
            data[i][B] = data[i][B-1];
            data[0][i] = data[1][i];
            data[B][i] = data[B-1][i];
        }
    }else if(condicion ==2){
        for(i=A;i<B;i++){
            data[i][0] = data[i][B-1];
            data[i][B] = data[i][1];
            data[0][i] = data[B-1][i];
            data[B][i] = data[1][i];
        }
    }else{
        for(i=A;i<B;i++){
            data[i][0] = 50.0;
            data[i][B] = 50.0;
            data[0][i] = 50.0;
            data[B][i] = 50.0;
        }
    }
}

void nombreArchivo(char filename[100], float t, int caso, int condicion){
    char cas[20];
    char con[20];
    char tiempo[20];
    
    if(caso == 1){
        strcpy(cas, "SinFuente");
    }else if(caso == 2){
        strcpy(cas, "ConFuente");
    }
    
    if(condicion ==1){
        strcpy(con, "Libre");
    }else if(condicion == 2){
        strcpy(con, "Periodicas");
    }else if(condicion == 3){
        strcpy(con, "Fijas");
    }
    
    if(t == 0.0){
        strcpy(tiempo, "0");
    }else if(t == 100.0){
        strcpy(tiempo, "100");
    }else if(t == 2500.0){
        strcpy(tiempo, "2500");
    }
    
    strcat(filename, cas);
    strcat(filename, con);
    strcat(filename, tiempo);
    strcat(filename,".dat");
}