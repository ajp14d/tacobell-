                                                     
 #include "MyProcesses.h"

extern struct beginPro* thequeue[];

void Init()
{
        int i = 0;
        while(i < THEQUEUE_SIZE)
        {
                thequeue[i] = NULL;
                i++;
        }


}

void handleQueue(struct beginPro* loc)
{
        int i = 0;
        // find first available slot
        while(i < THEQUEUE_SIZE)
        {
                if (checkNull(thequeue[i]))
                {
                        thequeue[i] = loc;
                        displayProStart(thequeue[i], i);
                        break;
                }

                i++;
        }

}

void checkQueue()
{
        int i = 0;
        while(i < THEQUEUE_SIZE)
        {
                if (!checkNull(thequeue[i]))
                {
                        if (checkPro(thequeue[i]))
                        {
                                remPro(i);
                        }
                }
                i++;
        }
}

void exitQueue()
{
        int run = 1;
        while(run)
        {
                checkQueue();
                int i = 0;
                while(i < THEQUEUE_SIZE)
                {
                        if (!checkNull(thequeue[i]))
                        {
                                run = 1;
                                break;
                        }
                        i++;
                }

                switch(i){
                  case THEQUEUE_SIZE:
                        run = 0;
                        break;
                }


        }
}

void remPro(int ind)
{
        if (ind >= 0)
        {
          if (ind < THEQUEUE_SIZE)
                if (!checkNull(thequeue[ind]))
                {
                        displayProStart(thequeue[ind], ind);
                        spacePro(thequeue[ind]);
                        thequeue[ind] = NULL;
                }
        }
}

struct beginPro* newPro(int firstpid, int secondpid, char* thing)
{
        struct beginPro* ans = (struct beginPro*)calloc(1, sizeof(struct beginPro));
        ans->firstpid = firstpid;
        ans->secondpid = secondpid;
        ans->thing = (char*)calloc(strlen(thing)+1,sizeof(char));
        strcpy(ans->thing, thing);
        return ans;
}
void spacePro(struct beginPro* pro)
{
        free(pro->thing);
        free(pro);
}

int checkPro(struct beginPro* pro)
{
        int now;
        int waitpid_ans;
        if (pro->firstpid != -1)
        {
                waitpid_ans = waitpid(pro->secondpid, &now, WNOHANG);

                switch(waitpid_ans){
                  case 0:
                        return 0;
                        break;
                  case -1:
                        printf("Error in child process: %s\n", pro->thing);
                        break;
                }

        }
        waitpid_ans = waitpid(p->firstpid, &now, WNOHANG);

        switch(waitpid_ans){
          case 0:
                return 0;
                break;
          case -1:
                printf("Error in child process: %s\n", pro->thing);
                break;
          default:
                return 1;
                break;
        }


        return 1;       // fix compiler warning
}

void displayProStart(const struct beginPro* pro, int loc)
{
        if (!checkNull(pro))
        {
                if (pro->second != -1)
                {
                        printf("[%i]\t[%i][%i]\n", loc, pro->firstpid, pro->secondpid);
                }
                else
                {
                        printf("[%i]\t[%i]\n", loc, pro->firstpid);
                }
        }
}

void displayProDone(const struct beginPro* pro, int loc)
{
        if (!checkNull(pro))
        {
                printf("[%i]+\t[%s]\n", loc, pro->thing);
        }
}

bool checkNull(const struct beginPro* tocheck)
{

        if(tocheck == NULL)
                return true;
        else
                return false;
}
