package main;

import javax.jws.WebService;

@WebService
public interface WebServiceInterface {

	public void print();

	public void setup(String o, String u, Double a);
}