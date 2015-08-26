<?php
use Tem\Controller;

class Home extends Controller{

    function __construct(){
        parent::__construct();
	$this->load->model("Welcome");
    }
    
    function index(){
              $this->load->view("home/index.php", $this->Welcome->get_data());
    }
    
}

