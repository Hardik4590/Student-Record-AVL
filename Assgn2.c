#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct student_list{
	char stu_name[100];
	char dept_name[100];
	int sem;
	char roll_no[100];
	int noc[5];
	int noca[5];
	struct student_list* left;
    struct student_list* right;
    int bal_fact;
}stu_list;
int mod(int x)
{
	if(x<0)
	{
		return (-1)*x;
	}
	else
	{
		return x;
	}
}
int height(stu_list* root)
{
	int ret_val;
	if(root==NULL)
	{
		ret_val=0;
	}
	else if(root->right==NULL && root->left==NULL)
	{
		ret_val=1;
	}
	else
	{
		int right_height,left_height;
		left_height=height(root->left);
		right_height=height(root->right);
		if(left_height>right_height)
		{
			ret_val=left_height+1;
		}
		else
		{
			ret_val=right_height+1;
		}
	}
	return ret_val;
}
void set_bf(stu_list* root)
{
	if(root->left==NULL && root->right==NULL)
		{
			root->bal_fact=0;
		}
		else if(root->left==NULL)
		{
			root->bal_fact=height(root->right);
		}
		else if(root->right==NULL)
		{
			root->bal_fact=0-height(root->left);
		}
		else
		{
			root->bal_fact=height(root->right)-height(root->left);
		}
}
stu_list* Rrotate(stu_list* root) {
    stu_list* new_root = root->left;
    stu_list* temp = new_root->right;
    
    // rotate
    new_root->right = root;
    root->left = temp;
    
    // update balance factors
    set_bf(root);
    set_bf(new_root);
    
    return new_root;
}
stu_list* Lrotate(stu_list* x) {
    stu_list* y = x->right;
    stu_list* t2 = y->left;
    
    // perform rotation
    y->left = x;
    x->right = t2;
    
    // update balance factors
    set_bf(x);
    set_bf(y);
    
    // return new root
    return y;
}

stu_list* insert(stu_list* root,char name[],char dept[],int sem,int cond[],int att[],char roll[])
{
	//printf("ENtered");
	if(root==NULL)
	{
		//printf("Entered");
		root=(stu_list*)malloc(sizeof(stu_list));
		//root->data=key;
		strcpy(root->stu_name,name);
		strcpy(root->dept_name,dept);
		root->sem=sem;
		int i;
		for(i=0;i<5;i++)
		{
			root->noc[i]=cond[i];
			root->noca[i]=att[i];
		}
		strcpy(root->roll_no,roll);
		root->bal_fact=0;
		root->left=root->right=NULL;
	}
	else
	{
		if(strcmp(roll,root->roll_no)<0)
		{
			root->left=insert(root->left,name,dept,sem,cond,att,roll);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			//printf("Entered");
			root->right=insert(root->right,name,dept,sem,cond,att,roll);
		}
		else
		{
			printf("Key already exists");
		}
	}
	//printf("Done");
	set_bf(root);
	if(root->bal_fact<-1 && root->left->bal_fact<=0)
	{
		root=Rrotate(root);
	}
	else if(root->bal_fact<-1 && root->left->bal_fact>0)
	{
		root->left=Lrotate(root->left);
		root=Rrotate(root);
	}
	else if(root->bal_fact>1 && root->right->bal_fact>=0)
	{
		root=Lrotate(root);
	}
	else if(root->bal_fact>1 && root->right->bal_fact<0)
	{
		root->right=Rrotate(root->right);
		root=Lrotate(root);
	}
	return root;
}
stu_list* search(stu_list* root,char roll[])
{
	stu_list* rv=NULL;
	if(root!=NULL)
	{
		//printf("Entered");
		if(strcmp(roll,root->roll_no)<0)
		{
			rv=search(root->left,roll);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			rv=search(root->right,roll);
		}
		else
		{
			//printf("Entered");
			rv=root;
		}
	}
	return rv;
}
void inorder(stu_list* root)
{
	if(root!=NULL)
	{
		inorder(root->left);
		printf("Name:%s\n",root->stu_name);
		printf("DEpt:%s\n",root->dept_name);
		printf("Sem:%d\n",root->sem);
		printf("RollNo:%s\n",root->roll_no);
		printf("%d\n",root->bal_fact);
		inorder(root->right);
	}
}

stu_list* delete_stu_list(stu_list* root, char roll[]) {
    stu_list* temp = NULL;
    
    if (root == NULL) {
        printf("Key not found");
    }
    else if (strcmp(roll, root->roll_no) < 0) {
        root->left = delete_stu_list(root->left, roll);
        set_bf(root);
    }
    else if (strcmp(roll, root->roll_no) > 0) {
        root->right = delete_stu_list(root->right, roll);
        set_bf(root);
    }
    else {
        if (root->left == NULL || root->right == NULL) {
            temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
        }
        else {
            temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy(root->stu_name, temp->stu_name);
            strcpy(root->dept_name, temp->dept_name);
            root->sem = temp->sem;
            strcpy(root->roll_no, temp->roll_no);
            int i;
            for ( i = 0; i < 5; i++) 
			{
                root->noc[i] = temp->noc[i];
                root->noca[i] = temp->noca[i];
            }
            root->right = delete_stu_list(root->right, temp->roll_no);
            set_bf(root);
        }
    }
    
    if (root != NULL) {
        if (root->bal_fact < -1 && root->left->bal_fact <= 0) {
            root = Rrotate(root);
        }
        else if (root->bal_fact < -1 && root->left->bal_fact > 0) {
            root->left = Lrotate(root->left);
            root = Rrotate(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact >= 0) {
            root = Lrotate(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact < 0) {
            root->right = Rrotate(root->right);
            root = Lrotate(root);
        }
    }
    
    return root;
}
stu_list* make_stu_list(stu_list* root,int n)
{
	//printf("ENtered");
	FILE* fp;
	fp=fopen("stu_list.txt","r");
	if(fp==NULL)
	{
		printf("File not opened\n");
		exit(0);
	}
	
	char name[100];
	char dept[100];
	int sem;
	int cond[5];
	int att[5];
	char roll[100];
	int i,j;
	for(j=0;j<n;j++)
	{
		//printf("entered%d\n",j);
		//printf("ENtered%d",j);
		fscanf(fp,"%s",name);
		fscanf(fp,"%s",dept);
		fscanf(fp,"%d",&sem);
		fscanf(fp,"%s",roll);
		for(i=0;i<5;i++)
		{
			fscanf(fp,"%d",&cond[i]);
		}
		for(i=0;i<5;i++)
		{
			fscanf(fp,"%d",&att[i]);
		}
		root=insert(root,name,dept,sem,cond,att,roll);
	}
	return root;
	
}



typedef struct Attendence_list{
	char roll_no[100];
	char dept_name[100];
	float att_MCC;
	float perc_att;
	struct Attendence_list* left;
    struct Attendence_list* right;
    int bal_fact;
}Attendence;
int height_att(Attendence* root)
{
	int ret_val;
	if(root==NULL)
	{
		ret_val=0;
	}
	else if(root->right==NULL && root->left==NULL)
	{
		ret_val=1;
	}
	else
	{
		int right_height,left_height;
		left_height=height_att(root->left);
		right_height=height_att(root->right);
		if(left_height>right_height)
		{
			ret_val=left_height+1;
		}
		else
		{
			ret_val=right_height+1;
		}
	}
	return ret_val;
}
void set_bf_att(Attendence* root)
{
	if(root->left==NULL && root->right==NULL)
		{
			root->bal_fact=0;
		}
		else if(root->left==NULL)
		{
			root->bal_fact=height_att(root->right);
		}
		else if(root->right==NULL)
		{
			root->bal_fact=0-height_att(root->left);
		}
		else
		{
			root->bal_fact=height_att(root->right)-height_att(root->left);
		}
}

Attendence* Rrotate_att(Attendence* root) {
    Attendence* new_root = root->left;
    Attendence* temp = new_root->right;
    
    // rotate
    new_root->right = root;
    root->left = temp;
    
    // update balance factors
    set_bf_att(root);
    set_bf_att(new_root);
    
    return new_root;
}
Attendence* Lrotate_att(Attendence* x) {
    Attendence* y = x->right;
    Attendence* t2 = y->left;
    
    // perform rotation
    y->left = x;
    x->right = t2;
    
    // update balance factors
    set_bf_att(x);
    set_bf_att(y);
    
    // return new root
    return y;
}
Attendence* insert_att(Attendence* root,char dept[],char roll[],int mcc_att)
{
	if(root==NULL)
	{
		//printf("Entered");
		root=(Attendence*)malloc(sizeof(Attendence));
		strcpy(root->dept_name,dept);
		strcpy(root->roll_no,roll);
		root->att_MCC=mcc_att;
		root->perc_att=0.0;
		root->bal_fact=0;
		root->left=root->right=NULL;
	}
	else
	{
		if(strcmp(roll,root->roll_no)<0)
		{
			root->left=insert_att(root->left,dept,roll,mcc_att);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			//printf("Entered");
			root->right=insert_att(root->right,dept,roll,mcc_att);
		}
		else
		{
			printf("Key already exists");
		}
	}
	set_bf_att(root);
	if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)<0)
	{
		root=Rrotate_att(root);
	}
	else if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)>0)
	{
		root->left=Lrotate_att(root->left);
		root=Rrotate_att(root);
	}
	else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)>0)
	{
		root=Lrotate_att(root);
	}
	else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)<0)
	{
		root->right=Rrotate_att(root->right);
		root=Lrotate_att(root);
	}
	return root;
}
Attendence* search_att(Attendence* root,char roll[])
{
	Attendence* rv=NULL;
	if(root!=NULL)
	{
		printf("Entered");
		if(strcmp(roll,root->roll_no)<0)
		{
			rv=search_att(root->left,roll);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			rv=search_att(root->right,roll);
		}
		else
		{
			//printf("Entered");
			rv=root;
		}
	}
	return rv;
}
void inorder_att(Attendence* root)
{
	if(root!=NULL)
	{
		inorder_att(root->left);
		//printf("Name:%s\n",root->stu_name);
		printf("DEpt:%s\n",root->dept_name);
		//printf("Sem:%d\n",root->sem);
		printf("RollNo:%s\n",root->roll_no);
		printf("ATT_MCC:%f\n",root->att_MCC);
		printf("Percentage attendence:%f\n",root->perc_att);
		printf("%d\n",root->bal_fact);
		inorder_att(root->right);
	}
}

Attendence* delete_Attendence_record(Attendence* root, char roll[]) {
    Attendence* temp = NULL;
    
    if (root == NULL) {
        printf("Key not found");
    }
    else if (strcmp(roll, root->roll_no) < 0) {
        root->left = delete_Attendence_record(root->left, roll);
        set_bf_att(root);
    }
    else if (strcmp(roll, root->roll_no) > 0) {
        root->right = delete_Attendence_record(root->right, roll);
        set_bf_att(root);
    }
    else {
        if (root->left == NULL || root->right == NULL) {
            temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
        }
        else {
            temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy(root->dept_name,temp->dept_name);
            root->att_MCC=temp->att_MCC;
            root->perc_att=temp->perc_att;
            strcpy(root->roll_no,temp->roll_no);
            root->right = delete_Attendence_record(root->right, temp->roll_no);
            set_bf_att(root);
        }
    }
    
    if (root != NULL) {
        if (root->bal_fact < -1 && root->left->bal_fact <= 0) {
            root = Rrotate_att(root);
        }
        else if (root->bal_fact < -1 && root->left->bal_fact > 0) {
            root->left = Lrotate_att(root->left);
            root = Rrotate_att(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact >= 0) {
            root = Lrotate_att(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact < 0) {
            root->right = Rrotate_att(root->right);
            root = Lrotate_att(root);
        }
    }
    
    return root;
}
Attendence* make_Attendence_list(Attendence* root,int n)
{
	FILE* fp;
	fp=fopen("Attendence.txt","r");
	if(fp==NULL)
	{
		printf("File not opened\n");
		exit(0);
	}
	char dept[100];
	char roll[100];
	float att_mcc;
	int j;
	for(j=0;j<n;j++)
	{
		//printf("ENtered%d",j);
		fscanf(fp,"%s",roll);
		fscanf(fp,"%s",dept);
		fscanf(fp,"%f",&att_mcc);
		//printf("%d\n",att_mcc);
		root=insert_att(root,dept,roll,att_mcc);
	}
	return root;
}


typedef struct fee_status_list{
	char roll_no[100];
	char status[100];
	struct fee_status_list* left;
	struct fee_status_list* right;
	int bal_fact;
}fee_status;

int height_fee(fee_status* root)
{
	int ret_val;
	if(root==NULL)
	{
		ret_val=0;
	}
	else if(root->right==NULL && root->left==NULL)
	{
		ret_val=1;
	}
	else
	{
		int right_height,left_height;
		left_height=height_fee(root->left);
		right_height=height_fee(root->right);
		if(left_height>right_height)
		{
			ret_val=left_height+1;
		}
		else
		{
			ret_val=right_height+1;
		}
	}
	return ret_val;
}
void set_bf_fee(fee_status* root)
{
	if(root->left==NULL && root->right==NULL)
		{
			root->bal_fact=0;
		}
		else if(root->left==NULL)
		{
			root->bal_fact=height_fee(root->right);
		}
		else if(root->right==NULL)
		{
			root->bal_fact=0-height_fee(root->left);
		}
		else
		{
			root->bal_fact=height_fee(root->right)-height_fee(root->left);
		}
}

fee_status* Rrotate_fee(fee_status* root) {
    fee_status* new_root = root->left;
    fee_status* temp = new_root->right;
    
    // rotate
    new_root->right = root;
    root->left = temp;
    
    // update balance factors
    set_bf_fee(root);
    set_bf_fee(new_root);
    
    return new_root;
}
fee_status* Lrotate_fee(fee_status* x) {
    fee_status* y = x->right;
    fee_status* t2 = y->left;
    
    // perform rotation
    y->left = x;
    x->right = t2;
    
    // update balance factors
    set_bf_fee(x);
    set_bf_fee(y);
    
    // return new root
    return y;
}
fee_status* insert_fee(fee_status* root,char roll[],char status[])
{
	if(root==NULL)
	{
		//printf("Entered");
		root=(fee_status*)malloc(sizeof(fee_status));
		strcpy(root->roll_no,roll);
		strcpy(root->status,status);
		root->bal_fact=0;
		root->left=root->right=NULL;
	}
	else
	{
		if(strcmp(roll,root->roll_no)<0)
		{
			root->left=insert_fee(root->left,roll,status);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			root->right=insert_fee(root->right,roll,status);
		}
		else
		{
			printf("Key already exists");
		}
	}
	set_bf_fee(root);
	if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)<0)
	{
		root=Rrotate_fee(root);
	}
	else if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)>0)
	{
		root->left=Lrotate_fee(root->left);
		root=Rrotate_fee(root);
	}
	else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)>0)
	{
		root=Lrotate_fee(root);
	}
	else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)<0)
	{
		root->right=Rrotate_fee(root->right);
		root=Lrotate_fee(root);
	}
	return root;
}
fee_status* search_fee(fee_status* root,char roll[])
{
	fee_status* rv=NULL;
	if(root!=NULL)
	{
		//printf("Entered");
		if(strcmp(roll,root->roll_no)<0)
		{
			rv=search_fee(root->left,roll);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			rv=search_fee(root->right,roll);
		}
		else
		{
			//printf("Entered");
			rv=root;
		}
	}
	return rv;
}
void inorder_fee(fee_status* root)
{
	if(root!=NULL)
	{
		inorder_fee(root->left);
		//printf("Name:%s\n",root->stu_name);
		//printf("DEpt:%s\n",root->dept_name);
		//printf("Sem:%d\n",root->sem);
		printf("RollNo:%s\n",root->roll_no);
		printf("Fee Status:%s\n",root->status);
		printf("%d\n",root->bal_fact);
		inorder_fee(root->right);
	}
}

fee_status* delete_fee_status_record(fee_status* root, char roll[]) {
    fee_status* temp = NULL;
    
    if (root == NULL) {
        printf("Key not found");
    }
    else if (strcmp(roll, root->roll_no) < 0) {
        root->left = delete_fee_status_record(root->left, roll);
        set_bf_fee(root);
    }
    else if (strcmp(roll, root->roll_no) > 0) {
        root->right = delete_fee_status_record(root->right, roll);
        set_bf_fee(root);
    }
    else {
        if (root->left == NULL || root->right == NULL) {
            temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
        }
        else {
            temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy(root->roll_no,temp->roll_no);
            strcpy(root->status,temp->status);
            root->right = delete_fee_status_record(root->right, temp->roll_no);
            set_bf_fee(root);
        }
    }
    
    if (root != NULL) {
        if (root->bal_fact < -1 && root->left->bal_fact <= 0) {
            root = Rrotate_fee(root);
        }
        else if (root->bal_fact < -1 && root->left->bal_fact > 0) {
            root->left = Lrotate_fee(root->left);
            root = Rrotate_fee(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact >= 0) {
            root = Lrotate_fee(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact < 0) {
            root->right = Rrotate_fee(root->right);
            root = Lrotate_fee(root);
        }
    }
    
    return root;
}

fee_status* make_fee_status_list(fee_status* root,int n)
{
	FILE* fp;
	fp=fopen("fee_status.txt","r");
	if(fp==NULL)
	{
		printf("File not opened\n");
		exit(0);
	}
	char status[100];
	char roll[100];
	int att_mcc;
	int j;
	for(j=0;j<n;j++)
	{
		fscanf(fp,"%s",roll);
		fscanf(fp,"%s",status);
		root=insert_fee(root,roll,status);
	}
	return root;
}



typedef struct applicant_list{
	char stu_name[100];
	char roll_no[100];
	char applied[10];
	struct applicant_list* left;
	struct applicant_list* right;
	int bal_fact;
}applicant;
int height_app(applicant* root)
{
	int ret_val;
	if(root==NULL)
	{
		ret_val=0;
	}
	else if(root->right==NULL && root->left==NULL)
	{
		ret_val=1;
	}
	else
	{
		int right_height,left_height;
		left_height=height_app(root->left);
		right_height=height_app(root->right);
		if(left_height>right_height)
		{
			ret_val=left_height+1;
		}
		else
		{
			ret_val=right_height+1;
		}
	}
	return ret_val;
}
void set_bf_app(applicant* root)
{
	if(root->left==NULL && root->right==NULL)
		{
			root->bal_fact=0;
		}
		else if(root->left==NULL)
		{
			root->bal_fact=height_app(root->right);
		}
		else if(root->right==NULL)
		{
			root->bal_fact=0-height_app(root->left);
		}
		else
		{
			root->bal_fact=height_app(root->right)-height_app(root->left);
		}
}

applicant* Rrotate_app(applicant* root) {
    applicant* new_root = root->left;
    applicant* temp = new_root->right;
    
    // rotate
    new_root->right = root;
    root->left = temp;
    
    // update balance factors
    set_bf_app(root);
    set_bf_app(new_root);
    
    return new_root;
}
applicant* Lrotate_app(applicant* x) {
    applicant* y = x->right;
    applicant* t2 = y->left;
    
    // perform rotation
    y->left = x;
    x->right = t2;
    
    // update balance factors
    set_bf_app(x);
    set_bf_app(y);
    
    // return new root
    return y;
}
applicant* insert_app(applicant* root,char name[],char roll[],char applied[])
{
	if(root==NULL)
	{
		//printf("Entered");
		root=(applicant*)malloc(sizeof(applicant));
		//strcpy(root->dept_name,dept);
		strcpy(root->stu_name,name);
		strcpy(root->roll_no,roll);
		strcpy(root->applied,applied);
		root->bal_fact=0;
		root->left=root->right=NULL;
	}
	else
	{
		if(strcmp(roll,root->roll_no)<0)
		{
			root->left=insert_app(root->left,name,roll,applied);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			//printf("Entered");
			root->right=insert_app(root->right,name,roll,applied);
		}
		else
		{
			printf("Key already exists");
		}
	}
	set_bf_app(root);
	if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)<0)
	{
		root=Rrotate_app(root);
	}
	else if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)>0)
	{
		root->left=Lrotate_app(root->left);
		root=Rrotate_app(root);
	}
	else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)>0)
	{
		root=Lrotate_app(root);
	}
	else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)<0)
	{
		root->right=Rrotate_app(root->right);
		root=Lrotate_app(root);
	}
	//printf("DOne");
	return root;
}
applicant* search_app(applicant* root,char roll[])
{
	applicant* rv=NULL;
	if(root!=NULL)
	{
		//printf("Entered");
		if(strcmp(roll,root->roll_no)<0)
		{
			rv=search_app(root->left,roll);
		}
		else if(strcmp(roll,root->roll_no)>0)
		{
			rv=search_app(root->right,roll);
		}
		else
		{
			//printf("Entered");
			rv=root;
		}
	}
	return rv;
}
void inorder_app(applicant* root)
{
	if(root!=NULL)
	{
		inorder_app(root->left);
		printf("Name:%s\n",root->stu_name);
		//printf("DEpt:%s\n",root->dept_name);
		//printf("Sem:%d\n",root->sem);
		printf("RollNo:%s\n",root->roll_no);
		printf("Applied or not:%s\n",root->applied);
		printf("%d\n",root->bal_fact);
		inorder_app(root->right);
	}
}

applicant* delete_applicant_record(applicant* root, char roll[]) {
    applicant* temp = NULL;
    
    if (root == NULL) {
        printf("Key not found");
    }
    else if (strcmp(roll, root->roll_no) < 0) {
        root->left = delete_applicant_record(root->left, roll);
        set_bf_app(root);
    }
    else if (strcmp(roll, root->roll_no) > 0) {
        root->right = delete_applicant_record(root->right, roll);
        set_bf_app(root);
    }
    else {
        if (root->left == NULL || root->right == NULL) {
            temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
        }
        else {
            temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy(root->roll_no,temp->roll_no);
            //strcpy(root->status,temp->status);
            strcpy(root->stu_name,temp->stu_name);
            strcpy(root->applied,temp->applied);
            root->right = delete_applicant_record(root->right, temp->roll_no);
            set_bf_app(root);
        }
    }
    
    if (root != NULL) {
        if (root->bal_fact < -1 && root->left->bal_fact <= 0) {
            root = Rrotate_app(root);
        }
        else if (root->bal_fact < -1 && root->left->bal_fact > 0) {
            root->left = Lrotate_app(root->left);
            root = Rrotate_app(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact >= 0) {
            root = Lrotate_app(root);
        }
        else if (root->bal_fact > 1 && root->right->bal_fact < 0) {
            root->right = Rrotate_app(root->right);
            root = Lrotate_app(root);
        }
    }
    
    return root;
}
applicant* make_applicant_list(applicant* root,int n)
{
	FILE* fp;
	fp=fopen("applicant.txt","r");
	if(fp==NULL)
	{
		printf("File not opened\n");
		exit(0);
	}
	char name[100];
	char roll[100];
	char applied[100];
	int j;
	for(j=0;j<n;j++)
	{
		//printf("App_inserted%d",j);
		fscanf(fp,"%s",name);
		fscanf(fp,"%s",roll);
		fscanf(fp,"%s",applied);
		root=insert_app(root,name,roll,applied);
	}
	return root;
}





//Percentage attendence
float Max_MCC_att(Attendence* root,float max)
{
	if(root!=NULL)
	{
		max=Max_MCC_att(root->left,max);
		if(root->att_MCC>max)
		{
			max=root->att_MCC;
		}
		max=Max_MCC_att(root->right,max);
	}
	return max;
}
void set_perc_att(Attendence *root,float max)
{
	if(root!=NULL)
	{
		set_perc_att(root->left,max);
		root->perc_att=(root->att_MCC/max)*100.00;
		set_perc_att(root->right,max);
	}
}






//Q1
stu_list* insert_sorted(stu_list* root,char name[],char dept[],int sem,int cond[],int att[],char roll[])
{
	if(root==NULL)
	{
		//printf("Entered");
		root=(stu_list*)malloc(sizeof(stu_list));
		//root->data=key;
		strcpy(root->stu_name,name);
		strcpy(root->dept_name,dept);
		root->sem=sem;
		int i;
		for(i=0;i<5;i++)
		{
			root->noc[i]=cond[i];
			root->noca[i]=att[i];
		}
		strcpy(root->roll_no,roll);
		root->bal_fact=0;
		root->left=root->right=NULL;
	}
	else
	{
		if(sem<root->sem)
		{
			root->left=insert_sorted(root->left,name,dept,sem,cond,att,roll);
		}
		else if(sem>root->sem)
		{
			root->right=insert_sorted(root->right,name,dept,sem,cond,att,roll);
		}
		else
		{
			if(strcmp(dept,root->dept_name)<0)
			{
				root->left=insert_sorted(root->left,name,dept,sem,cond,att,roll);
			}
			else if(strcmp(dept,root->dept_name)>0)
			{
				root->right=insert_sorted(root->right,name,dept,sem,cond,att,roll);
			}
			else
			{
				if(strcmp(roll,root->roll_no)<0)
				{
					root->left=insert_sorted(root->left,name,dept,sem,cond,att,roll);
				}
				else if(strcmp(roll,root->roll_no)>0)
				{
					root->right=insert_sorted(root->right,name,dept,sem,cond,att,roll);
				}
				else
				{
					printf("Student already present\n");
				}
			}
		}
	}
	set_bf(root);
	if(root->bal_fact<-1 && sem<root->left->sem)
	{
		root=Rrotate(root);
	}
	else if(root->bal_fact<-1 && sem>root->left->sem)
	{
		root->left=Lrotate(root->left);
		root=Rrotate(root);
	}
	else if(root->bal_fact<-1 && sem==root->left->sem)
	{
		if(root->bal_fact<-1 && strcmp(dept,root->left->dept_name)<0)
		{
			root=Rrotate(root);
		}
		else if(root->bal_fact<-1 && strcmp(dept,root->left->dept_name)>0)
		{
			root->left=Lrotate(root->left);
			root=Rrotate(root);
		}
		else if(root->bal_fact<-1 && strcmp(dept,root->left->dept_name)==0)
		{
			if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)<0)
			{
				root=Rrotate(root);
			}
			else if(root->bal_fact<-1 && strcmp(roll,root->left->roll_no)>0)
			{
				root->left=Lrotate(root->left);
				root=Rrotate(root);
			}
		}
	}
	else if(root->bal_fact>1 && sem>root->right->sem)
	{
		root=Lrotate(root);
	}
	else if(root->bal_fact>1 && sem<root->right->sem)
	{
		root->right=Rrotate(root->right);
		root=Lrotate(root);
	}
	else if(root->bal_fact>1 && sem==root->right->sem)
	{
		if(root->bal_fact>1 && strcmp(dept,root->right->dept_name)>0)
		{
			root=Lrotate(root);
		}
		else if(root->bal_fact>1 && strcmp(dept,root->right->dept_name)<0)
		{
			root->right=Rrotate(root->right);
			root=Lrotate(root);
		}
		else if(root->bal_fact>1 && strcmp(dept,root->right->dept_name)==0)
		{
			if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)>0)
			{
				root=Lrotate(root);
			}
			else if(root->bal_fact>1 && strcmp(roll,root->right->roll_no)<0)
			{
				root->right=Rrotate(root->right);
				root=Lrotate(root);
			}
		}
	}
	return root;
}
stu_list* sort(stu_list* root,stu_list* nroot)
{
	//stu_list* nroot;
	if(root!=NULL)
	{
		//printf("ENtered");
		nroot=sort(root->left,nroot);
		nroot=insert_sorted(nroot,root->stu_name,root->dept_name,root->sem,root->noc,root->noca,root->roll_no);
		//printf("Inserted");
		nroot=sort(root->right,nroot);
	}
	return nroot;
}



//Q2
stu_list* not_applied(stu_list* stu_root,applicant* app_root,stu_list* rv)
{
	//stu_list* rv=NULL:
	if(app_root!=NULL)
	{
		rv=not_applied(stu_root,app_root->left,rv);
		if(strcmp(app_root->applied,"NA")==0)
		{
			stu_list* node=search(stu_root,app_root->roll_no);
			rv=insert_sorted(rv,node->stu_name,node->dept_name,node->sem,node->noc,node->noca,node->roll_no);
			//printf("%s\n",rv->stu_name);
		}
		rv=not_applied(stu_root,app_root->right,rv);
	}
	return rv;
}




//Q3
stu_list* eligible(stu_list* stu_root,Attendence* att_root,fee_status* fee_root,applicant*app_root,stu_list* rv)
{
	
	if(att_root!=NULL)
	{
		rv=eligible(stu_root,att_root->left,fee_root,app_root,rv);
		if(att_root->perc_att>75.00)
		{
			
			fee_status* node=search_fee(fee_root,att_root->roll_no);
			if(strcmp(node->status,"clear")==0)
			{
				applicant* node1=search_app(app_root,node->roll_no);
				if(strcmp(node1->applied,"A")==0)
				{
					stu_list* node2=search(stu_root,node1->roll_no);
					rv=insert_sorted(rv,node2->stu_name,node2->dept_name,node2->sem,node2->noc,node2->noca,node2->roll_no);
				}
			}
			
		}
		rv=eligible(stu_root,att_root->right,fee_root,app_root,rv);
	}
	return rv;
}



//Q4
stu_list* less_attendence(stu_list* stu_root,Attendence* att_root,stu_list* rv)
{
	//stu_list* rv=NULL:
	//inorder(stu_root);
	if(att_root!=NULL)
	{
		rv=less_attendence(stu_root,att_root->left,rv);
		if(att_root->perc_att<=75.00)
		{
			//printf("Entered");
			//printf("%s\n",att_root->roll_no);
			stu_list* node=search(stu_root,att_root->roll_no);
			//printf("%s\n",node->stu_name);
			rv=insert_sorted(rv,node->stu_name,node->dept_name,node->sem,node->noc,node->noca,node->roll_no);
			//printf("%s\n",rv->stu_name);
		}
		rv=less_attendence(stu_root,att_root->right,rv);
	}
	return rv;
}


//Q5
void delete_record(stu_list* stu_root,Attendence* att_root,fee_status* fee_root,applicant* app_root)
{
	char roll[100];
	printf("Enter the roll number you want to delete:");
	scanf("%s",roll);
	stu_root=delete_stu_list(stu_root,roll);
	//printf("Deleted from stu_list");
	att_root=delete_Attendence_record(att_root,roll);
	fee_root=delete_fee_status_record(fee_root,roll);
	app_root=delete_applicant_record(app_root,roll);
}




//Q6
void status_pending(stu_list* stu_root,Attendence* att_root,fee_status* fee_root)
{
	//stu_list* rv=NULL:
	//inorder(stu_root);
	if(att_root!=NULL)
	{
		status_pending(stu_root,att_root->left,fee_root);
		if(att_root->perc_att>75.00)
		{
			
			fee_status* node=search_fee(fee_root,att_root->roll_no);
			if(strcmp(node->status,"pending")==0)
			{
				stu_list* node1=search(stu_root,node->roll_no);
				printf("%s\n",node1->stu_name);
			}
			
		}
		status_pending(stu_root,att_root->right,fee_root);
	}
}






//Q7
stu_list* defaulter_by_attendence(stu_list* stu_root,Attendence* att_root,stu_list* rv)
{
	if(att_root!=NULL)
	{
		rv=defaulter_by_attendence(stu_root,att_root->left,rv);
		if(att_root->perc_att<=75.00)
		{
			stu_list* node=search(stu_root,att_root->roll_no);
			rv=insert_sorted(rv,node->stu_name,node->dept_name,node->sem,node->noc,node->noca,node->roll_no);
		}
		rv=defaulter_by_attendence(stu_root,att_root->right,rv);
	}
	return rv;
}
stu_list* defaulter_by_fee_status(stu_list* stu_root,Attendence* att_root,fee_status* fee_root,stu_list* rv)
{
	if(fee_root!=NULL)
	{
		rv=defaulter_by_fee_status(stu_root,att_root,fee_root->left,rv);
		if(strcmp(fee_root->status,"Pending")==0)
		{
			Attendence* node=search_att(att_root,fee_root->roll_no);
			if(node->perc_att>75)
			{
				stu_list* node1=search(stu_root,node->roll_no);
				rv=insert_sorted(rv,node1->stu_name,node1->dept_name,node1->sem,node1->noc,node1->noca,node1->roll_no);
			}
		}
		rv=defaulter_by_fee_status(stu_root,att_root,fee_root->right,rv);
	}
	return rv;
}
void max_defaulters(stu_list* root,int arr[])
{
	if(root!=NULL)
	{
		max_defaulters(root->left,arr);
		if(strcmp(root->dept_name,"CS")==0)
		{
			arr[0]++;
		}
		else if(strcmp(root->dept_name,"EC")==0)
		{
			arr[1]++;
		}
		else if(strcmp(root->dept_name,"EE")==0)
		{
			arr[2]++;
		}
		else if(strcmp(root->dept_name,"ME")==0)
		{
			arr[3]++;
		}
		else if(strcmp(root->dept_name,"CV")==0)
		{
			arr[4]++;
		}
		max_defaulters(root->right,arr);
	}
}
void max_index(int arr[])
{
	int max=0;
	int i,max_i=0;
	for(i=0;i<5;i++)
	{
		if(arr[i]>max)
		{
			max=arr[i];
			max_i=i;
		}
	}
	if(max_i==0)
	{
		printf("CS has max number of defaulters\n");
	}
	else if(max_i==1)
	{
		printf("EC has max number of defaulters\n");
	}
	else if(max_i==2)
	{
		printf("EE has max number of defaulters\n");
	}
	else if(max_i==3)
	{
		printf("ME has max number of defaulters\n");
	}
	else if(max_i==4)
	{
		printf("CV has max number of defaulters\n");
	}
}





//Q8
void range_search(stu_list* root,char roll1[],char roll2[])
{
	if(root!=NULL)
	{
		if(strcmp(root->roll_no,roll1)>0)
		{
			range_search(root->left,roll1,roll2);
		}
		if(strcmp(root->roll_no,roll1)>0 && strcmp(root->roll_no,roll2)<0)
		{
			printf("%s\n",root->stu_name);
		}
		if(strcmp(root->roll_no,roll2)<0)
		{
			range_search(root->right,roll1,roll2);
		}
		
	}
}




int main()
{
	int n;
	printf("ENter the number of students(Enter 100):");
	scanf("%d",&n);
	stu_list* root=NULL;
	root=make_stu_list(root,n);
	//inorder(root);
	printf("\n\n\n");
	Attendence* att_root=NULL;
	att_root=make_Attendence_list(att_root,n);
	//inorder_att(att_root);
	fee_status* fee_root=NULL;
	fee_root=make_fee_status_list(fee_root,n);
	//inorder_fee(fee_root);
	applicant* app_root=NULL;
	app_root=make_applicant_list(app_root,n);
	//inorder_app(app_root);
	float max=0.0;
	max=Max_MCC_att(att_root,max);
	//printf("Max:%f",max);
	set_perc_att(att_root,max);
	//inorder_att(att_root);
	int choice=0;
	while(choice!=9)
	{
		printf("1.Q1\n");
		printf("2.Q2\n");
		printf("3.Q3\n");
		printf("4.Q4\n");
		printf("5.Q5\n");
		printf("6.Q6\n");
		printf("7.Q7\n");
		printf("8.Q8\n");
		printf("9.exit\n");
		printf("Enter the choice:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 :
				{
					stu_list* nroot=NULL;
					nroot=sort(root,nroot);
					inorder(nroot);
					break;
				}
			case 2 :
				{
					stu_list* root4=NULL;
					root4=not_applied(root,app_root,root4);
					inorder(root4);
					break;
				}
			case 3 :
				{
					stu_list* elg=NULL;
					elg=eligible(root,att_root,fee_root,app_root,elg);
					inorder(elg);
					break;
				}
			case 4 :
				{
					stu_list* temp=NULL;
					temp=less_attendence(root,att_root,temp);
					stu_list* temp1=NULL;
					temp1=sort(temp,temp1);
					inorder(temp1);
					break;
				}
			case 5 :
				{
					delete_record(root,att_root,fee_root,app_root);
					inorder(root);
					inorder_att(att_root);
					inorder_fee(fee_root);
					inorder_app(app_root);
					break;
				}
			case 6 :
				{
					status_pending(root,att_root,fee_root);
					break;
				}
			case 7 :
				{
					stu_list* defaulter=NULL;
					defaulter=defaulter_by_attendence(root,att_root,defaulter);
					defaulter=defaulter_by_fee_status(root,att_root,fee_root,defaulter);
					inorder(defaulter);
					int arr[5]={0};
					max_defaulters(defaulter,arr);
					max_index(arr);
					break;
				}
			case 8 :
				{
					char roll1[100],roll2[100];
					printf("Enter Roll1:");
					scanf("%s",roll1);
					printf("Enter Roll2");
					scanf("%s",roll2);
					range_search(root,roll1,roll2);
					break;
				}
			case 9  :
				{
					break;
				}
			default :
				{
					printf("Check your choice\n");
				}
		}
	}
	
}
