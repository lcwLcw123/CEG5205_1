using AsyncIO;
using NetMQ;
using NetMQ.Sockets;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;


/// <summary>
///     Example of requester who only sends Hello. Very nice guy.
///     You can copy this class and modify Run() to suits your needs.
///     To use this class, you just instantiate, call Start() when you want to start and Stop() when you want to stop.
/// </summary>
public class HelloRequester : RunAbleThread
{
    /// <summary>
    ///     Request Hello message to server and receive message back. Do it 10 times.
    ///     Stop requesting when Running=false.
    /// </summary>

    public string message = null;
    public string last_motivation ="stop";
    public string motivation ="stop";
    public int t = 0;
    public override void Run()
    {
        ForceDotNet.Force(); // this line is needed to prevent unity freeze after one use, not sure why yet
        using (RequestSocket client = new RequestSocket())
        {
            client.Connect("tcp://localhost:5540");
            // for (int i = 0; i < 10 && Running; i++)
            while(Running)
            {
                // Debug.Log("Sending Hello");
                client.SendFrame("");
                bool gotMessage = false;
                while (Running)
                {
                    gotMessage = client.TryReceiveFrameString(out message); // this returns true if it's successful
                    if (gotMessage) break;
                }
                if(message == last_motivation){
                    t =t+1;
                    if (t>2){
                        if (gotMessage) Debug.Log("Received " + last_motivation);
                        t=0;
                    }
                }
                last_motivation = message;
                // if (gotMessage) Debug.Log(motivation+t);
                
            }
        }

        NetMQConfig.Cleanup(); // this line is needed to prevent unity freeze after one use, not sure why yet
    }
}