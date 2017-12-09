#include "management.h"

int main(){
    int exit=0;
    int login_flag=0;
    int temppw_flag = 0;
    
    Create_Struct();
   
    initscr();

    while(!exit){ 
        clear();
        
        switch(Account_Manage()){
            clear();
            case MENU_LOGIN :
                clear();
                printw("Student Number : ");
                scanw("%s", Curr_Num);
                
                printw("Password : ");
                noecho();
                scanw("%s", Curr_Pass);
                echo();

                login_flag = login();
                if(login_flag == 1){
                    clear();
                    switch(menu()){
                        case MENU_ASSIGN :
                            break;
                        
                        case MENU_CGPA :
                            switch(cgpa_menu()){
                                case CGPA_ADD : 
                                    break;
                                
                                case CGPA_VIEW : 
                                    break;
                                
                                case CGPA_QUIT : 
                                    break;
                                
                                default : 
                                    clear();
                                    printw("Wrong Key\n");
                                    printw("Press any key to return\n");
                                    getch();
                                    
                            }
                            break;                            
    
                        case MENU_CHANGE :
                            break;
                        
                        case MENU_LOGOUT :
                            break;
                        
                        default :
                            clear();
                            printw("Wrong Key\n");
                            printw("Press any key to return\n");
                            getch();
                            break;
                    } 
                }
                else{
                    clear();
                    printw("Wrong Information\n");
                    printw("Press any key to return\n");
                    getch();
                }
                break;
            
            case MENU_NEW : 
                break;
            
            case MENU_DEL :
                break;
            
            case MENU_TEMP :
                temppw_flag = Temp_Password();
                if(temppw_flag == 0){
                    clear();
                    printw("Wrong Information\n");
                    printw("Press any key to return\n");
                }
                getch();
                break;
            
            case MENU_QUIT :
                exit = 1;
                break;
            
            default :
                clear();
                printw("Wrong Key\n");
                printw("Press any key to return\n");
                getch();
                break;
        }
    }
    endwin();
//    Save_Data();
    system("clear");
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


    YEAR *head_YEAR = NULL, *cur_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL;
    CGPA *head_CGPA = NULL, *cur_CGPA = NULL;
    ASSIGN *head_ASSIGN = NULL, *cur_ASSIGN = NULL;

    fprintf(fpoint, "%d\n", TOP->Year_Size);

    head_YEAR = TOP->ST_YEAR;
    cur_YEAR = head_YEAR;
    
    for(i = 0; i < TOP->Year_Size; i++){
        fprintf(fpoint, "%s %d\n", cur_YEAR->year, cur_YEAR->Num_Size);

        if(i == 0){
            head_STUDENT = head_YEAR->ST_NUM;
            cur_STUDENT = head_STUDENT;
        }
        for(j = 0; j < cur_YEAR->Num_Size; j++){
            fprintf(fpoint, "%4s%4s\n", cur_YEAR->year, cur_STUDENT->number);
            fprintf(fpoint, "%s\n", cur_STUDENT->password);
            fprintf(fpoint, "%d %d\n", cur_STUDENT->Assign_Size, cur_STUDENT->CGPA_Size);
            
            head_ASSIGN = cur_STUDENT->Child_A;
            cur_ASSIGN = head_ASSIGN;
            head_CGPA = cur_STUDENT->Child_C;
            cur_CGPA = head_CGPA;
            
            for(k = 0; k < cur_STUDENT->Assign_Size; k++){
                fprintf(fpoint, "%s/%s/%s/%d %d\n", cur_ASSIGN->name, cur_ASSIGN->describe, cur_ASSIGN->professor, cur_ASSIGN->date[0], cur_ASSIGN->date[1]);
                cur_ASSIGN = cur_ASSIGN->link;    
            }
        
            for(k = 0; k < cur_STUDENT->CGPA_Size; k++){
                fprintf(fpoint, "%d %.2f\n", cur_CGPA->semester, cur_CGPA->score);
                cur_CGPA = cur_CGPA->link;    
            }
            cur_STUDENT = cur_STUDENT->link;
        }
        cur_YEAR = cur_YEAR->link;
        fprintf(fpoint, "\n");
    }

    fclose(fpoint);

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


    TOP = malloc(sizeof(TREE_HEAD));
    fscanf(fpoint, "%d", &TOP->Year_Size);
    
    
    for(i = 0; i < TOP->Year_Size; i++){
        new_YEAR = malloc(sizeof(YEAR));
        fscanf(fpoint, "%s %d", new_YEAR->year, &new_YEAR->Num_Size);

        if(head_YEAR == NULL){
            head_YEAR = new_YEAR;
            TOP->ST_YEAR = head_YEAR; 
            tail_YEAR = new_YEAR;
        }
        else{
            tail_YEAR->link = new_YEAR;
            tail_YEAR = new_YEAR;
        }
        
        for(j = 0; j < new_YEAR->Num_Size; j++){
            new_STUDENT = malloc(sizeof(STUDENT));
            fscanf(fpoint, "%*4s%4s\n", new_STUDENT->number);
            fscanf(fpoint, "%s\n", new_STUDENT->password);
            fscanf(fpoint, "%d %d\n", &new_STUDENT->Assign_Size, &new_STUDENT->CGPA_Size);
            
            if(head_STUDENT == NULL){
                head_STUDENT = new_STUDENT;    
                head_YEAR->ST_NUM = head_STUDENT;
                tail_STUDENT = new_STUDENT;    
            }
            else{
                tail_STUDENT->link = new_STUDENT;
                tail_STUDENT = new_STUDENT;
            }
           
            head_ASSIGN = new_STUDENT->Child_A;
            tail_ASSIGN = new_STUDENT->Child_A;
            head_CGPA = new_STUDENT->Child_C;
            tail_CGPA = new_STUDENT->Child_C;
    
            for(k = 0; k < new_STUDENT->Assign_Size; k++){
                new_ASSIGN = malloc(sizeof(ASSIGN));
                fscanf(fpoint, "%100[^/] %*c %100[^/] %*c %100[^/] %*c %d %d\n", new_ASSIGN->name, new_ASSIGN->describe, new_ASSIGN->professor, &new_ASSIGN->date[0], &new_ASSIGN->date[1]);
                
                if(head_ASSIGN == NULL){
                    head_ASSIGN = new_ASSIGN;
                    new_STUDENT->Child_A = head_ASSIGN;
                    tail_ASSIGN = new_ASSIGN;
                }
                else{
                    tail_ASSIGN->link = new_ASSIGN;
                    tail_ASSIGN = new_ASSIGN;
                }
            }
        
            for(k = 0; k < new_STUDENT->CGPA_Size; k++){
                new_CGPA = malloc(sizeof(CGPA));
                fscanf(fpoint, "%d %f\n", &new_CGPA->semester, &new_CGPA->score);
                
                if(head_CGPA == NULL){
                    head_CGPA = new_CGPA;
                    new_STUDENT->Child_C = head_CGPA;
                    tail_CGPA = new_CGPA;
                }
                else{
                    tail_CGPA->link = new_CGPA;
                    tail_CGPA = new_CGPA;
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
    
    for(i = 0; i < 4; i++)
        year[i] = Curr_Num[i];
    for(i = 4; i < 8; i++)
        num[i - 4] = Curr_Num[i];//copying st number

    head_YEAR = TOP->ST_YEAR;
    cur_YEAR = head_YEAR;
    
    for(i = 0; i < TOP->Year_Size; i++){
        if(!strcmp(cur_YEAR->year, year)){
            year_flag = 1;
            break;
        }
        cur_YEAR = cur_YEAR->link;
    }
    if(year_flag == 1){
        head_STUDENT = head_YEAR->ST_NUM;
        cur_STUDENT = head_STUDENT;
    
        for(j = 0; j < cur_YEAR->Num_Size;j++){
            if(!strcmp(cur_STUDENT->number, num)){
                num_flag = 1;
                break;
            }
            cur_STUDENT = cur_STUDENT->link;
        }
    }
    if(num_flag == 1){
        if(!strcmp(cur_STUDENT->password, Curr_Pass)){
            pass_flag = 1;
           // Login_Year = i;//stores logged in user's year
           // Login_Num = j;//stores logged in user's number
            node_login_Year = cur_YEAR;
            node_login_Num = cur_STUDENT;
        }
    }
    if(pass_flag==1)
        return 1;
    return 0;
}

int Temp_Password(){
    int flag1=0;
    int year_flag = 0, num_flag = 0;//added->to check info
    int i; //j, k; used linked list, no need to use repeats
    char num[5]={};
    char year[5]={};
    int temp_num;//to make random Password
    char temppassword[8];
    float temp;

    clear();
    echo();
    srand(time(NULL));
    printw("Student Number:");
    scanw("%s",Curr_Num);
    
    for(i = 0; i < 4; i++)
        year[i] = Curr_Num[i];
    for(i = 4; i < 8; i++)
        num[i - 4] = Curr_Num[i];//copying st number

    
    head_YEAR = TOP->ST_YEAR;
    cur_YEAR = head_YEAR;
    
    
    for(i = 0; i < TOP->Year_Size; i++){
        if(!strcmp(cur_YEAR->year, year)){
            year_flag = 1;
            break;
        }
        cur_YEAR = cur_YEAR->link;
    }
    

    if(year_flag == 1){
        head_STUDENT = head_YEAR->ST_NUM;
        cur_STUDENT = head_STUDENT;
    
        for(i = 0; i < cur_YEAR->Num_Size; i++){
            if(!strcmp(cur_STUDENT->number, num)){
                num_flag = 1;
                break;
            }
            cur_STUDENT = cur_STUDENT->link;
        }
    }
    
    
    if(num_flag == 1){
        printw("Input latest semester's GPA : ");
        scanw("%f", &temp);
        
        head_CGPA = cur_STUDENT->Child_C;
        cur_CGPA = head_CGPA;
        
        for(i = 0; i < cur_STUDENT->CGPA_Size - 1; i++)
            cur_CGPA = cur_CGPA->link;

        if(cur_CGPA->score == temp) flag1 = 1;
    }


    if(flag1 == 1){
        for(i = 0; i < 8; i++){
            temp_num = rand() % 93 + 21;
            temppassword[i] = (char)temp_num;
        }
        printw("Temporary Password is <%s>\n", temppassword);
        strcpy(cur_STUDENT->password, temppassword);
        return 1;
    }

    return 0;
}
