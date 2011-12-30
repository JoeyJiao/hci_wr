#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int hci_w(int reg, int val)
{
    int returnval = 0;

    char s1[100] = "hcitool cmd 0x3f 0x15 ";
    char stemp[10] = "";
    char starget[100] = "";
    char *pstarget = starget;

    sprintf(stemp, "0x%x ", reg);
    pstarget = strcat(s1, stemp);

    sprintf(stemp, "0x%x ", 0);
    pstarget = strcat(pstarget, stemp);

    sprintf(stemp, "0x%x ", val);
    pstarget = strcat(pstarget, stemp);
    returnval = system(pstarget);
    return returnval;
}

int hci_r(int reg)
{
    FILE* returnval;
    int ulval;
    char s1[100] = "hcitool cmd 0x3f 0x15 ";
    char stemp[10] = "";
    char starget[100] = "";
    char reading[200] = "";
    char *pstarget = starget;
    char *returnv;

    sprintf(stemp, "0x%x ", reg);
    pstarget=strcat(s1, stemp);

    sprintf(stemp, "0x%x ", 1);
    pstarget=strcat(pstarget, stemp);

    sprintf(stemp, "0x%x ", 1);
    pstarget = strcat(pstarget, stemp);

    returnval = popen(pstarget,"r");

    if(!returnval){
      printf("Could not open pipe for output.\n");
      return 0;
    }

    // Grab data from process execution
    // Skip the first 3 lines
    fgets(reading, 200 , returnval);
    fgets(reading, 200 , returnval);
    fgets(reading, 200 , returnval);
    fgets(reading, 200 , returnval);

    if (pclose(returnval) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");

    returnv = strndup(reading + (strlen(reading)-4), 2);
    ulval= strtoul(returnv, NULL, 16);
    //LOGD("hci_r 0x%x \n", ulval);
    return ulval;
}

void usage()
{
	printf("\n[USAGE:]hci_wr r(/w/a) reg [value]\n");
}

int main(int argc, char **argv)
{
	unsigned reg,val,ret;
	int i;
	if(argc==1 || (argc==2 && strcmp(argv[1],"a")) || (argc==3 && strcmp(argv[1],"r")) || (argc==4 && strcmp(argv[1],"w")) || argc>4){
		usage();
		return -1;
	}
	if(!strcmp(argv[1],"a")){
		printf("hci_r=\n");
		for(i=0;i<=0xff;i++){
			ret=hci_r(i);
			printf("0x%x\t",ret);
		}
		printf("\n");
		return 0;
	}
	reg=strtoul(argv[2],0,10);
	if(!strcmp(argv[1],"r")){
		ret=hci_r(reg);	
		printf("\nhci_r[0x%x]=0x%x\n",reg,ret);
	} else if (!strcmp(argv[1],"w")){
		val=strtoul(argv[3],0,10);
		int i=hci_w(reg,val);
		if(i!=0){
			printf("\nhci_w error, return %d\n",i);
			return i;
		}
		printf("\nhci_r[0x%x]=0x%x\n",reg,hci_r(reg));
	}
	return 0;
}

