#include<stdio.h>
#include<string.h>

char data[3][4];
int RollPitchYaw[3];

void getData(unsigned char *line)
{
    char c = line[0];
    int i = 0;
    int n = 0;
    int dataIndex = 0;
    while(c != '\n')
    {
        if(c != ',')
        {
            data[n][dataIndex] = c;
            i++;
            dataIndex++;
        }
        else if(c == ',')
        {
            RollPitchYaw[n] = atoi(data[n]);
            n++;
            i++;
            dataIndex = 0;
        }
        c = line[i];
    }
    RollPitchYaw[2] = atoi(data[2]);
}

int main()
{
    unsigned char line[20] = "-254,-345,-90\n";
    printf("%s\n",line);
    getData(line);
    int i=0;
    int j=0;
    int a = RollPitchYaw[0];
    int b = RollPitchYaw[1];
    int c = RollPitchYaw[2];
    printf("%d %d %d\n",a,b,c);

}
