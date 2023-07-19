#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;
                  



int main(int argc, char *argv[])
{

 if(argc != 2)
    {
        cerr << "Greska pri vnes na argumenti...\n" << endl;
        exit(0);
    }
     

    
    
    int port = atoi(argv[1]);

      if (port != 57555)
      {
         cerr<<"Greska pri vnes na porta...\n" <<endl;
         exit(0);
      }

     
     
   
     char msg[500];


   


    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // so htonl se pravi konverzija na unsigned 32 b od host byte order во network byte order
    // od INADDR_ANY koristime nekoja wildcard adresa sto raspolaga samiot toj
    servAddr.sin_port = htons(port);
 
   

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);


    if(serverSd < 0)
    {
        cerr << "Greska pri vospostavuvanje na serverski soket...\n" << endl;
        exit(0);
    }


    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
   

 if(bindStatus < 0)
    {
        cerr << "Greska pri vospostavuvanje na soketska adresa...\n" << endl;
        exit(0);
    }


    cout << "Cekanje klientot da se povrzi...\n" << endl;
    

    if(listen(serverSd, 4) < 0 )
    {
         
         cerr <<"Greska pri slusanje na soketot"<<endl;

    }

    cout << "Ocekuvam baranja za ostvaruvanje na konekcija...\n"<< endl;


    sockaddr_in client;
    socklen_t clientSize = sizeof(client);


 
   
    int newSd = accept(serverSd, (sockaddr *)&client, &clientSize);

   
if(newSd < 0)
    {
        cerr << "“Nastana greska. Obidete se povtorno! ... \n" << endl;
        exit(1);
    }

   cout << "Povrzani ste so klientot...\n" << endl;
    

 while(1)
    {
      
        cout << "Cekanje odgovor od klientot...\n" << endl;
        
       
         memset(&msg, 0, sizeof(msg)); 
        
        
        
        recv(newSd, (char*)&msg, sizeof(msg), 0);


        cout<<"Poraka od klientot e...\n"<< msg <<endl;

       
        if(!strcmp(msg, "EXIT1"))
        {
            cout << "Kraj na vrska...\n" << endl;
            break;
        }
        
        
        cout << "Red na Serverot..." <<endl;

    
      
        string data;

        int lenmsg = strlen(msg);

 

         

       int brojac = 0 ;

       bool tocno = false;

        
        for (int i = 0 ; i <lenmsg-1;i++)
         {
            if(isdigit(msg[i]))
            {
               tocno = true;
               if(msg[i] == '0')
               {
                 msg[i] = 'a';
               }
               else
               {
               msg[i] = 'a' + (msg[i] - '0');  // ako e broj = msg[1] = a(e 97) + (msg[1] - 0(e 48)) a e 97 a 0 e 48 sea i++ se inkrementira i doaga 1 toa e 98 i 49
                                              // ako e broj = msg[2] = b(e 98 zaradi i so ++) +(msg[2] - 1 (e 49 zaradi i so e ++)) i t aka natamamu

               }  

               brojac ++ ;
               

         }

         else
            {
                if(msg[brojac] == 0)
                {

                     tocno = false;
                }
                
               
            }
        }  
           
           
           

         
       
           
               

          
           cout << "Prerabotenata poraka e" << msg <<endl;


         
 

         if(tocno == true)
         {

            data = "Ima vkupno "+to_string(brojac)+" broevi vo porakata";
            memset(&msg, 0, sizeof(msg)); 
            strcpy(msg, data.c_str());
            send(newSd, (char*)&msg, strlen(msg), 0);

         }
         else
         {
         if(tocno == false)
         {
            data = "Vasata poraka se sostoi samo od bukvi";
            memset(&msg, 0, sizeof(msg)); 
            strcpy(msg, data.c_str());
            send(newSd, (char*)&msg, strlen(msg), 0);


         }
         }
        
        
     
    }
     
     
    close(newSd);
    close(serverSd);

    return 0;   
}
