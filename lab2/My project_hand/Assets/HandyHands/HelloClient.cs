using UnityEngine;

public class HelloClient : MonoBehaviour
{
    private HelloRequester _helloRequester;
    public float speed = 1.5f;//控制移动速度
    public Transform m_transform;
    private void Start()
    {
        _helloRequester = new HelloRequester();
        m_transform = this.transform; 
        _helloRequester.Start();
    }
    private void Update()
    {
        if (_helloRequester.last_motivation == "left"){
        m_transform.Translate(Vector3.left * Time.deltaTime * speed);
        }

        //right
        if (_helloRequester.last_motivation == "right")
        {

            m_transform.Translate(Vector3.right * Time.deltaTime * speed);
        }
        //forward
        if (_helloRequester.last_motivation == "foaward")
        {

            m_transform.Translate(Vector3.forward * Time.deltaTime * speed);
        }
        //back
        if (_helloRequester.last_motivation == "back")
        {

            m_transform.Translate(Vector3.back * Time.deltaTime * speed);
        }
        //up
        if (_helloRequester.last_motivation == "up")
        {

            m_transform.Translate(Vector3.up * Time.deltaTime * speed);
        }
         //below
        if (_helloRequester.last_motivation == "down")
        {

            m_transform.Translate(Vector3.down * Time.deltaTime * speed);
        }

    }
    private void OnDestroy()
    {
        _helloRequester.Stop();
    }
}