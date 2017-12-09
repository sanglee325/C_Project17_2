#include "management.h"

int main()
{
    int exit=0;
    int login_flag=0;
    int i, j, k;
    
    YEAR *head_YEAR = NULL, *cur_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL;
    CGPA *head_CGPA = NULL, *cur_CGPA = NULL;
    ASSIGN *head_ASSIGN = NULL, *cur_ASSIGN = NULL;

    Create_Struct();

   
    printf("%d\n", TOP->Year_Size);

    head_YEAR = TOP->ST_YEAR;
    cur_YEAR = head_YEAR->link;
    
    
    head_STUDENT = head_YEAR->ST_NUM;
    cur_STUDENT = head_STUDENT->link;
    
    for(i = 0; i < TOP->Year_Size; i++){
        printf("%s %d\n", cur_YEAR->year, cur_YEAR->Num_Size);
        
        for(j = 0; j < cur_YEAR->Num_Size; j++){
            printf("%4s%4s\n", cur_YEAR->year, cur_STUDENT->number);
            printf("%s\n", cur_STUDENT->password);
            printf("%d %d\n", cur_STUDENT->Assign_Size, cur_STUDENT->CGPA_Size);
            
            head_ASSIGN = cur_STUDENT->Child_A;
            cur_ASSIGN = head_ASSIGN->link;
            head_CGPA = cur_STUDENT->Child_C;
            cur_CGPA = head_CGPA->link;
            
            for(k = 0; k < cur_STUDENT->Assign_Size; k++){
                printf("%s/%s/%s/%d %d\n", cur_ASSIGN->name, cur_ASSIGN->describe, cur_ASSIGN->professor, cur_ASSIGN->date[0], cur_ASSIGN->date[1]);
                cur_ASSIGN = cur_ASSIGN->link;    
            }
        
            for(k = 0; k < cur_STUDENT->CGPA_Size; k++){
                printf("%d %.2f\n", cur_CGPA->semester, cur_CGPA->score);
                cur_CGPA = cur_CGPA->link;    
            }
            cur_STUDENT = cur_STUDENT->link;
        }
        printf("\n");
    
        cur_YEAR = cur_YEAR->link;
    }
  /*  initscr();

    while(!exit){ 
        clear();
        switch(Account_Manage()){
            case MENU_LOGIN :
                login();
                break;
            case MENU_NEW : 
                break;
            case MENU_DEL :
                break;
            case MENU_TEMP :
                break;
            case MENU_QUIT :
                break;
        }
    }
    endwin();
    Save_Data();
    system("clear");*/
    return 0;
}
void Save_Data()
{
    int i,j,k;
    int current_year;
    int current_num;
    int current_assign;
    int current_cgpa;
    FILE* fpoint;
    fpoint=fopen("data.txt","w");




}
void Create_Struct()
{
    int i,j,k;
    int exit = 0, flag_exit;
    int current_year;
    int current_num;
    int current_assign;
    int current_cgpa;
    char* token;
    char temp[350];
    char temp_year[5], temp_num[5], temp_snum[9];
    FILE* fpoint = fopen("data.txt", "r");

    YEAR *head_YEAR = NULL, *new_YEAR = NULL, *cur_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *new_STUDENT, *cur_STUDENT = NULL;
    CGPA *head_CGPA = NULL, *new_CGPA, *cur_CGPA = NULL;
    ASSIGN *head_ASSIGN = NULL, *new_ASSIGN, *cur_ASSIGN = NULL;

    TOP = malloc(sizeof(TREE_HEAD));
    fscanf(fpoint, "%d", &TOP->Year_Size);
    
    TOP->ST_YEAR = malloc(sizeof(YEAR));
    head_YEAR = TOP->ST_YEAR;
    
    head_YEAR->ST_NUM = malloc(sizeof(STUDENT));
    head_STUDENT = head_YEAR->ST_NUM;

    for(i = 0; i < TOP->Year_Size; i++){
        new_YEAR = malloc(sizeof(YEAR));
        fscanf(fpoint, "%s %d", new_YEAR->year, &new_YEAR->Num_Size);

        if(head_YEAR->link == NULL){
            head_YEAR->link = new_YEAR;    
        }
        else{
            new_YEAR->link = head_YEAR->link;
            head_YEAR->link = new_YEAR;
        }
        
        new_STUDENT = malloc(sizeof(STUDENT));
        for(j = 0; j < new_YEAR->Num_Size; j++){
            head_STUDENT = head_YEAR->ST_NUM;
            fscanf(fpoint, "%*4s%4s\n", new_STUDENT->number);
            fscanf(fpoint, "%s\n", new_STUDENT->password);
            fscanf(fpoint, "%d %d\n", &new_STUDENT->Assign_Size, &new_STUDENT->CGPA_Size);
            
            if(head_STUDENT->link == NULL){
                head_STUDENT->link = new_STUDENT;    
            }
            else{
                new_STUDENT->link = head_STUDENT->link;
                head_STUDENT->link = new_STUDENT;
            }
           
            new_STUDENT->Child_A = malloc(sizeof(ASSIGN));
            head_ASSIGN = new_STUDENT->Child_A;
            new_STUDENT->Child_C = malloc(sizeof(CGPA));
            head_CGPA = new_STUDENT->Child_C;
    
            for(k = 0; k < new_STUDENT->Assign_Size; k++){
                new_ASSIGN = malloc(sizeof(ASSIGN));
                fscanf(fpoint, "%100[^/] %*c %100[^/] %*c %100[^/] %*c %d %d\n", new_ASSIGN->name, new_ASSIGN->describe, new_ASSIGN->professor, &new_ASSIGN->date[0], &new_ASSIGN->date[1]);
                
                if(head_ASSIGN->link == NULL){
                    head_ASSIGN->link = new_ASSIGN;    
                }
                else{
                    new_ASSIGN->link = head_ASSIGN->link;
                    head_ASSIGN->link = new_ASSIGN;
                }
            }
        
            for(k = 0; k < new_STUDENT->CGPA_Size; k++){
                new_CGPA = malloc(sizeof(CGPA));
                fscanf(fpoint, "%d %f\n", &new_CGPA->semester, &new_CGPA->score);
                
                if(head_CGPA->link == NULL){
                    head_CGPA->link = new_CGPA;    
                }
                else{
                    new_CGPA->link = head_CGPA->link;
                    head_CGPA->link = new_CGPA;
                }
            }
        }
        flag_exit = fscanf(fpoint, "\n");
        if(flag_exit == EOF) break;
    }

    fclose(fpoint);
}
char Account_Manage()
{
    printw("1. Login\n");
    printw("2. Create new Account\n");
    printw("3. Delete Account\n");
    printw("4. Get Temporary Password\n");
    printw("5. Exit\n");
    return wgetch(stdscr);
}
char menu()
{
    printw("1. Assignment Management\n");
    printw("2. CGPA Management\n");
    printw("3. Change Password\n");
    printw("4. Logout\n");
    return wgetch(stdscr);
}
void Search_Assign()
{
    int Asize;
    char input;
    clear();
    printw("<Assignment Management for %s>\n",Curr_Num);
    Asize = TOP->ST_YEAR[Login_Year].ST_NUM[Login_Num].Assign_Size;
    Sort_Assign();
    Print_Assign(Asize);
    printw("1. New Assignment\n2. Delete Assignment\n3. Return to main menu\n");
    noecho();
    input=wgetch(stdscr);
    switch(input)
    {
        case '1': Add_Assign(); break;
        case '2': Delete_Assign(); break;
        case '3': return; break;
    }
    clear();
}

void Print_Assign(int Asize)
{
    int k,D_day,thistime,thattime;
    time_t t;
    struct tm *today;
    int i;
    int j;
    i=Login_Year;
    j=Login_Num;
    t = time(NULL);
    today = localtime(&t);
    thistime = mktime(today);
    /*
       To do...
       */


}

char cgpa_menu()
{
    printw("1. Add new GPA or Change existing GPA\n");
    printw("2. View all\n");
    printw("3. Exit\n");
    return wgetch(stdscr);
}
void Search_CGPA() 
{
    int exit;
    Curr_year_index=Login_Year;
    Curr_num_index=Login_Num;
    while(!exit)
    {
        clear();
        switch(cgpa_menu())
        {
            case CGPA_ADD: Add_GPA(); break;
            case CGPA_VIEW: Print_CGPA(); break;
            case CGPA_QUIT: exit = 1; break;
            default: break;
        }
    }
    exit = 0;
}
void Add_GPA()
{
    char semester;
    float gpa;
    int gpa_size;
    clear();
    echo();
    printw("Input the semester of GPA : ");
    scanw("%c", &semester);

    gpa_size = TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size;  

    if(semester - '0' > (gpa_size + 1) || semester - '0' < 1) {
        printw("You entered wrong number!\n");
        getch();
        return ;
    } 
    printw("Input the GPA : ");
    scanw("%f", &gpa);

    if(gpa < 0 || gpa > 4.31) {
        printw("You entered wrong number!\n");
        getch();
        return ;
    }

    if(semester - '0' <= gpa_size) {
        Cor_GPA(semester, gpa);
        return ;
    }

    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size++;
    gpa_size++;

    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C = (CGPA*)realloc(TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C,gpa_size * sizeof(CGPA));

    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[gpa_size - 1].semester = semester - '0';
    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[gpa_size - 1].score = gpa; 

    return ;

}
void Cor_GPA(char semester, float gpa) 
{
    int i;
    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[semester - '0' - 1].score = gpa;
    printw("GPA is changed successfully!\n");
    getch();
    return ;

}
void Print_CGPA() //Additional
{
    int i;
    int tmpSemester;
    float tmpGPA;
    float sum = 0;
    clear();
    printw("<CGPA Management for %s>\n", Curr_Num);
    printw("------------------------------\n");
    printw("  Semester  |  GPA  |  CGPA  |  \n");
    for(i = 0 ; i < TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size ; i++) 
    {
        /*
           To do...
           */
        printw("------------------------------\n");
        printw("     %d      |  %.2f |  %.2f  |\n", tmpSemester, tmpGPA, sum/(i + 1));
    }
    printw("------------------------------\n\n");
    printw("<CGPA Management by Graph for %s>\n", Curr_Num);
    printw("(x-axis : semester,  y-axis : score)\n");
    Print_CGPA_Graph();
    getch();
}
void Print_CGPA_Graph() 
{
    /*
       To do...(Additional)
       */
}
void Sort_Assign()
{

    /*

       To do...

*/
}
void Add_Assign()
{
    //Login_Num,Login_Year
    /*
       To do...

*/
}
void Delete_Assign()
{
    /*
       To do...

*/
}
void Add_CGPA()
{
    //Login_Num,Login_Year

    /*
       To do...
       */  

}
void New_Account()
{
    /*
       To do...
       */

}
void Delete_Account()
{
    /*
       To do...
       */

}
void Change_Password()
{

    /*
       To do...
       */

}

int login()
{
    int i,j;
    int year_flag=0;
    int num_flag=0;
    int pass_flag=0;
    char year[5]={};
    char num[5]={};
    for(i=0;i<4;i++)
        year[i]=Curr_Num[i];
    for(i=4;i<8;i++)
        num[i-4]=Curr_Num[i];
    for(i=0;i<TOP->Year_Size;i++)
    {
        if(!strcmp(TOP->ST_YEAR[i].year,year))
        {
            year_flag=1;
            break;
        }
    }
    if(year_flag==1)
    {
        for(j=0;j<TOP->ST_YEAR[i].Num_Size;j++)
        {
            if(!strcmp(TOP->ST_YEAR[i].ST_NUM[j].number,num))
            {
                num_flag=1;
                break;
            }
        }
    }
    if(num_flag==1)
    {
        if(!strcmp(TOP->ST_YEAR[i].ST_NUM[j].password,Curr_Pass))
        {
            pass_flag=1;
            Login_Year=i;//stores logged in user's year
            Login_Num=j;//stores logged in user's number
        }
    }
    if(pass_flag==1)
        return 1;
    return 0;
}

void Temp_Password()
{
    int flag1=0;
    int i, j, k;
    char num[5]={};
    char temppassword[8];
    float temp;
    clear();
    echo();
    srand(time(NULL));
    printw("Student Number:");
    scanw("%s",Curr_Num);
    /*
       To do...
       */
}
