//
//  ViewController.swift
//  iostest
//
//  Created by Gust on 2017/10/6.
//  Copyright © 2017年 Gust. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let myString = "in main!"
        print(myString)
        
        
        let mainBundle = Bundle.main.bundlePath
        let app_path=mainBundle.cString(using: String.Encoding.utf8)
        call_jvm(UnsafeMutablePointer<Int8>(mutating: app_path));

    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

