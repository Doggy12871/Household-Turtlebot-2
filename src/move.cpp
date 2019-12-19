#include "ros/ros.h"
#include "std_msgs/String.h"
#include "math.h" // ��֪��ros֧��֧�� 
 
/**
 * �����Ҿ�������ע���ˣ������۵��ǲ�����
 * �ܹ�������һ��subscriber���������������룬�������Ƿ��ҵ�����������0��1����ʾ
 * �ڶ�������Ӧ�����˵ķ���ţ������������м򵥵�ͳ�ƽ�ģ����û�и�������ѧϰģ�ͣ� 
 * ��3��4������Ӧ�����˵�λ��x��y���Ҳ�̫����Ǹ��㻹�����Σ����Ȱ��ո��㴦��
 * ��5��6�������ǻ����˵ĵ�ǰλ�ã�x1��y1�������� 
 * Ԥ�Ƶ��������ַ�����ʽ���磺"1 4 144.23 353.2 100.04 88.54" �� "0 -1 0 0 100.04 88.54" 
 */
 
 // TODO: 1��ʵ���˶�����move() 2��ʵ����ת����rotateSearch() 3����¼ÿ�������е�λ��middleX()��middleY()
 // ע�⣺Ŀǰ��rotate��move��ԭ�ӵģ��ж�rotate�Ƿ���ɵĵط���Ҫ�Ľ� 

/**
 * �����ע�⵽�������Ϊģʽ���ж��Ƿǳ��򵥵�ͳ����ʽ�ģ���һ���ֿ����޸� 
 */
long long modelCnt[6] = {0};
long long overallCnt = 0;
int searched[6] = {0}; // �����Ѿ��������ķ��� 
int lastRoom = 0; // ���۲⵽�˵ķ��䣬������Ŀ�귿�� 
int lastX = 0; // ���۲⵽�˵����꣬������Ŀ������ 
int lastY = 0;
bool rotate = 0; // rotate������¼�������Ƿ��Ѿ�������Ѱ����������ѰӦ��������ʧ�ķ���ʼת��
 
/** 
 * ����Ļص��Ƕ��յ���Ϣ�Ĵ�����Ԥ���й��ɵ��յ�λ����Ϣ 
 */
void positionCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("receive position info: [%s]", msg->data.c_str()); // debug string
  
  std::stringstream ss;
  ss << msg->data.c_str();
  int state, room;
  double x, y, x1, y1;
  ss >> state >> room >> x >> y >> x1 >> y1;
  
  if (state == 1)
  {
  	overallCnt++;
  	modelCnt[room]++;
  	for(int i = 0; i < 6; ++i)
	  searched[i] = 0;
	lastRoom = room;
	lastX = x;
	lastY = y;
	rotate = 0;
  	// ����ҵ����ˣ���ô�������֧��ֱ�����˵ķ�����
	moveTo(room, x, y, x1, y1); // ��Ҫ����һ��������������л������˶����� 
  }
  else
  {
  	if (fabs(x1 - lastX) > 1 || fabs(y - lastY) > 1) // ��û���� 
  		moveTo(lastRoom, lastX, lastY, x1, y1);
  	else if (rotate == 0)
  	{
	  rotateSearch(); // ������Ѱ 
	  rotate = 1; // ��Ҫ�Ľ����������������ת��һȦ��ʱ���Ϊ1������rotateSearch�������ϼ�1����Ϊ����rotateSearchһ�ε��ò�����תһȦ 
    }
  	else if (rotate == 1)
  	{
  	  searched[lastRoom] == 1; // ��������󿴵��˵�λ�ã�����ת��һȦ��û�����ˣ������Ѿ��ҹ����� 
      // ��Ұ��û�г����ˣ�����ȥ����ʧ�ķ�����
      int mostFrequent = -1;
      int frequency = 0;
      for (int i = 0; i < 6; ++i)
      {
        if (searched[i] == 1)
          continue;
		if (modelCnt[i] > frequency)
		{
		  mostFrequent = i;
		  frequency = modelCnt[i];
		}
	  }
	  if (mostFrequnt = -1)
	  {
	  	for(int i = 0; i < 6; ++i)
	      searched[i] = 0;
	    int mostFrequent = -1;
        int frequency = 0;
        for (int i = 0; i < 6; ++i)
        {
          if (searched[i] == 1)
            continue;
		  if (modelCnt[i] > frequency)
		  {
		    mostFrequent = i;
		    frequency = modelCnt[i];
		  }
	    }
	  }
	  // ��������Ӧ���ҵ���һ����û��������ȥ����Ƶ���ĵط� 
	  lastRoom = mostFrequent;
	  lastX = middleX(mostFrequent);
	  lastY = middleY(mostFrequent);
	  moveTo(lastRoom, lastX, lastY, x1, y1); // û�о�������ʱ��Ԥ��middleX��Y���ط����м��ַ 
    }
  }
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "move");
 
  ros::NodeHandle n;
  
  // ����������Ӧ����һ������position��topic 
  ros::Subscriber sub = n.subscribe("position", 1000, positionCallback);
 
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();
 
  return 0;
}
