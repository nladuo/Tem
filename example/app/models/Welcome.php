<?php
use Tem\Model;

class Welcome extends Model{
    
    function __construct(){
        parent::__construct();
        //you can load other models here
        //$this->load->model("Other model")
    }
    
    function get_data(){
        return [
            "title" => "TinyExtMvc",
            "content" => "this is an example of TinyExtMvc"
        ];
    }
    
}

