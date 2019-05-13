#include<iostream>
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>

using namespace std;
//retorna el par que contiene i y j del numero mayor
pair<int,int> mayor(vector<vector<float> > matrix, vector<bool> filas, vector<bool> columnas)
{
    int i,j;
    pair<int,int> par;
    //no pueden haber distancias negativas asi que se le asigna el menor valor que es 0
    float mayor=0;
    par.first=0;
    par.second=0;
    for(i=0; i<matrix.size(); i++)
    {
        for(j=0; j<matrix[i].size() && filas[i]; j++)
        {
            if(columnas[j] && matrix[i][j]!=0 && matrix[i][j]>mayor)
            {
                par.first=i;
                par.second=j;
                mayor=matrix[i][j];
            }
        }
    }
    return par;
}
//hace lo mismo que mayor solo que busca el menor
pair<int,int> menor(vector<vector<float> > matrix, vector<bool> filas, vector<bool> columnas)
{
    int i,j;
    pair<int,int> par;
    float menor=5000;
    par.first=0;
    par.second=0;
    for(i=0; i<matrix.size(); i++)
    {
        for(j=0; j<matrix[i].size() && filas[i]; j++)
        {
            if(columnas[j] && matrix[i][j]!=0 && matrix[i][j]<menor)
            {
                par.first=i;
                par.second=j;
                menor=matrix[i][j];
            }
        }
    }
    return par;
}
void impr(vector<vector<float> > matrix, vector<bool> filas, vector<bool> columnas)
{
    int i,j;
    for(i=0; i<matrix.size(); i++)
    {
        if(filas[i])
        {
            for(j=0; j<matrix[i].size(); j++)
            {
                if(columnas[j] || matrix[i][j]==0)
                {
                    printf("%.2f ",matrix[i][j]);
                }
            }
            cout<<endl;
        }

    }
}
//se calcula la distacia entre 2 puntos
float dist(pair<float,float> a,pair<float,float> b)
{
    float suma=0;
    suma+=pow(a.first-b.first,2);
    suma+=pow(a.second-b.second,2);
    return sqrt(suma);
}
//se comprueba si existe el numero a en el arreglo vec
bool exist(vector<int> vec,int a)
{
    int i;
    for(i=0; i<vec.size(); i++)
    {
        if(vec[i]==a)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    int i,j,kha,n,tipo,op,zelda,celda;
    float tam,y0,y1,x0,x1,x,y;
    cout<<"Ingrese la cantidad de personas"<<endl;
    cin>>n;
    printf("Ingrese el orden de la grilla\n");
    cin>>zelda;
    printf("Ingrese el tamaño de las celdas\n");
    cin>>tam;

    //este vector guarda los pares de filas y columnas que se han ido eliminando en el proceso
    //luego nos dimos cuenta que es inecesario, pero para que modificar el codiigo si asi funciona
    vector<pair<int,int> > pares;

    //guarda la ubicacion de cada persona
    vector<pair<float,float> > puntos(n);

    //esta es la matriz de distancias con la cual trabaja el algoritmo
    vector<vector<float> > matrix(n);

    //aqui se guardan los clusters que existen en cada etapa
    //es un "cubo" ya que para cada etapa se quiere guardar las personas que conforman el cluster de cada persona
    vector<vector<vector<int> > > clusters;

    //guarda los k de cada persona
    vector<int> k(n);

    //este guarda la id, dato que tambien es innecesario
    vector<int> id(n);

    //este vector almacena las personas que se encuentran en un cluster, se utiliza para determinar la region de encubrimiento
    vector<int> personas;

    //este vector va guardando que filas aun estan activas y las que se han eliminado
    vector<bool> filas(n,true);

    //lo mismo pero con las columnas
    vector<bool> columnas(n,true);
    pair<int,int> parcito;

    //leemos los datos de las n personas
    for(i=0; i<n; i++)
    {
        printf("Ingrese la ubicacion de la persona %d",i+1);
        printf("X: ");
        cin>>puntos[i].first;
        printf("Y: ");
        cin>>puntos[i].second;
        printf("Ingrese k\n");
        cin>>k[i];
        printf("Ingrese la ID de su celda\n");
        cin>>id[i];
    }

    //aqui procedemos a calcular las distancias
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            if(i!=j)
            {
                matrix[i].push_back(dist(puntos[i],puntos[j]));
            }
            else
            {
                //cuando i == j significa que se esta viendo la distancia de una persona consigo misma
                //entonces esa distancia es 0 y el resto no importa, asi que se cierra el ciclo
                matrix[i].push_back(0);
                break;
            }
        }
    }

    impr(matrix,filas,columnas);
    cout<<endl;
    printf("Que tipo de encubrimiento desea hacer, los mas cercanos(1) o los mas lejanos (2)\n");
    cin>>tipo;
    //si se elige los mas lejanos es mas probable que la region de encubrimiento sea casi toda la grilla y casi siempre la misma region
    //para cualquier persona
    for(i=0; i<n; i++)
    {
        //dependiendo del tipo, buscamos el menor o el mayor
        if(tipo==1)
        {
            parcito=menor(matrix,filas,columnas);
        }
        else
        {
            parcito=mayor(matrix,filas,columnas);
        }
        //si se retorno el par 0,0 significa que terminó el algoritmo ya que no puede generar mas uniones
        if(parcito.first==0 && parcito.second==0)
        {
            break;
        }
        //lo insertamos en pares
        pares.push_back(parcito);
        //y "eliminamos" la fila y columna
        filas[parcito.first]=false;
        columnas[parcito.second]=false;
        //imprimimos para ver como va quedando
        impr(matrix,filas,columnas);
        cout<<endl;
    }

    //le asignamos el tamaño
    clusters.resize(pares.size()+1);
    for(i=0;i<pares.size()+1;i++){
        clusters[i].resize(n);
    }
    for(i=0;i<n;i++)
    {
        //a la etapa 0 todos estan separados, asi que se le ingresa su propio numero
        clusters[0][i].push_back(i);
    }

    //empezamos desde 1 ya que la etapa 0 está lista
    for(i=1;i<pares.size()+1;i++)
    {
        //este for itera en la cantidad de personas
        for(j=0;j<n;j++)
        {
            //en la etapa i, de la persona j le asignamos el resultado de la etapa anterior
            //ya que puede mantenerse igual que el anterior o añadir gente a su cluster
            clusters[i][j]=clusters[i-1][j];
            //si en el cluster que estamos viendo existe la persona equivalente a la columna eliminada
            if(exist(clusters[i][j],pares[i-1].second))
            {
                //agregamos todas las personas que se encuentran en el cluster de la persona equivalente a la fila eliminada
                for(kha=0;kha<clusters[i-1][pares[i-1].first].size();kha++)
                {
                    //no deberian existir en el cluster que estamos viendo, pero de todas formas preguntamos, por seguridad
                    if(!exist(clusters[i][j],clusters[i-1][pares[i-1].first][kha]))
                    {
                        //y los insertamos
                        clusters[i][j].push_back(clusters[i-1][pares[i-1].first][kha]);
                    }
                }
            }
            //es identico aca solo que si antes era con la columna aqui es con fila y viceversa
            if(exist(clusters[i][j],pares[i-1].first))
            {
                for(kha=0;kha<clusters[i-1][pares[i-1].second].size();kha++)
                {
                    if(!exist(clusters[i][j],clusters[i-1][pares[i-1].second][kha]))
                    {
                        clusters[i][j].push_back(clusters[i-1][pares[i-1].second][kha]);
                    }
                }
            }
        }
    }

    //esto es solo imprime de los clusteres por etapa
    for(i=0;i<clusters.size();i++)
    {
        for(j=0;j<clusters[i].size();j++)
        {
            for(kha=0;kha<clusters[i][j].size();kha++)
            {
                printf("%d",clusters[i][j][kha]);
                if(kha!=clusters[i][j].size()-1)
                {
                    printf(",");
                }
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");

    //para cada persona buscamos su cluster con el k deseado
    for(i=0; i<n; i++)
    {

        for(j=0; j<pares.size()+1; j++)
        {
            //buscamos un cluster donde pertenece la persona i que sea de tamaño k o mayor
            if(clusters[j][i].size()>=k[i])
            {
                personas=clusters[j][i];
            }
        }
        //si personas esta vacio significa que habian clusters de menor tamaño
        if(personas.empty())
        {
            printf("El cluster donde se encuentra la persona %d es muy pequeño, se unirá con otro\n",i+1);
            //asi que le asignamos el cluster de la ultima etapa
            personas=clusters[pares.size()][i];
            for(j=0;j<n;j++)
            {
                //buscamos una persona que no exista en el cluster
                if(!exist(personas,j))
                {
                    //cuando la encontramos, fusionamos el cluster de la persona i con el de la persona j
                    for(kha=0;kha<clusters[pares.size()][j].size();kha++)
                    {
                        if(!exist(personas,clusters[pares.size()][j][kha]))
                        {
                            personas.push_back(clusters[pares.size()][j][kha]);
                        }
                    }
                }
                //si alcanzamos el tamaño o lo pasamos, terminamos
                if(personas.size()>=k[i])
                {
                    break;
                }
            }
        }
        //para calcular la region de encubrimiento usamos la ubicacion de las personas que se encuentran en el cluster
        //necesitamos la posicion X mayor y menor, ademas de la Y mayor y menor
        //con esos datos se puede formar una region cuadrada que cubre todos los puntos del cluster
        //sabiendo el tamaño de las celdas, podemos determinar facilmente cual es la region de encubrimiento
        x0=puntos[personas[0]].first;
        x1=puntos[personas[0]].first;
        y0=puntos[personas[0]].second;
        y1=puntos[personas[0]].second;

        //dentro de este for  se buscan los mayores y menores
        for(j=0;j<personas.size();j++)
        {
            if(puntos[personas[j]].first<x0){
                x0=puntos[personas[j]].first;
            }
            if(puntos[personas[j]].first>x1){
                x1=puntos[personas[j]].first;
            }
            if(puntos[personas[j]].second<y0){
                y0=puntos[personas[j]].second;
            }
            if(puntos[personas[j]].second>y1){
                y1=puntos[personas[j]].second;
            }
        }

        printf("Region de encubrimiento persona %d: ",i+1);
        //se empieza desde el Y menor hasta donde termina la celda del Y mayor
        //si y0=0.8,y1=7.8 y tamaño de la celda 3
        //ceil(y1/tam)*tam dara como resultado 9
        //ya que 7.8/3= 2.6, al aplicarle ceil queda en 3, y 3*3 es 9, siendo este el limite al cual puede llegar como maximo y0
        for(y=y0;y<=ceil(y1/tam)*tam;y+=tam)
        {
            //lo mismo para los x
            for(x=x0;x<=ceil(x1/tam)*tam;x+=tam)
            {
                //la celda se calcula como se calcula el indice cuando se pasan los datos de una matriz a un vector
                //(y/tam) seria la fila de la matriz
                //(x/tam) la columna de la matriz
                //entonces la fila se multiplica por la cantidad de columnas que tiene la matriz (zelda)
                //y se le suma la columna en que se encuentra y en este caso ademas se le suma 1 ya que comienza desde el 1 en adelante
                celda=(int)(y/tam)*zelda+(int)(x/tam)+1;
                printf("%d ",celda);

            }
        }
        printf("\n\n");
        personas.clear();
    }


}
