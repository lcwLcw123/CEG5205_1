using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class camerafloo : MonoBehaviour
{
    public GameObject ball;
    Vector3 distance;
    // Start is called before the first frame update
    void Start()
    {
        distance = ball.GetComponent<Transform>().position - gameObject.GetComponent<Transform>().position;
    }

    // Update is called once per frame
    void Update()
    {
        gameObject.GetComponent<Transform>().position = ball.GetComponent<Transform>().position - distance;
    }
}
