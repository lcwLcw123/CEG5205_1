using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class control : MonoBehaviour
{   
    public float speed = 1.5f;//控制移动速度
    public Transform m_transform;
    // Start is called before the first frame update
    void Start()
    {
        m_transform = this.transform; //当前游戏物体身上.获取Transform组件并用weizhi来保存它的引用
    }

    // Update is called once per frame
    void Update()
    {
        //left
        if (Input.GetKey(KeyCode.A))
        {

            m_transform.Translate(Vector3.left * Time.deltaTime * speed);
        }

        //right
        if (Input.GetKey(KeyCode.D))
        {

            m_transform.Translate(Vector3.right * Time.deltaTime * speed);
        }
        //forward
        if (Input.GetKey(KeyCode.W))
        {

            m_transform.Translate(Vector3.forward * Time.deltaTime * speed);
        }
        //back
        if (Input.GetKey(KeyCode.S))
        {

            m_transform.Translate(Vector3.back * Time.deltaTime * speed);
        }
        //up
        if (Input.GetKey(KeyCode.UpArrow))
        {

            m_transform.Translate(Vector3.up * Time.deltaTime * speed);
        }
         //below
        if (Input.GetKey(KeyCode.DownArrow))
        {

            m_transform.Translate(Vector3.down * Time.deltaTime * speed);
        }

    }
}
