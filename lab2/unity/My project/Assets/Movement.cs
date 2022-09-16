using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement : MonoBehaviour
{
    private Rigidbody rd;
    public int force =5;
    // Start is called before the first frame update
    void Start()
    {
        rd = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        float h =Input.GetAxis("Horizontal");
        float v=Input.GetAxis("Vertical");
        rd.AddForce(new Vector3(h,0,v)*force);
        
    }
}
