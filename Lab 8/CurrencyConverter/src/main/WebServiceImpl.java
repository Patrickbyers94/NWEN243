package main;

import javax.jws.WebService;

@WebService(
		endpointInterface = "main.WebServiceInterface",
		portName = "webservicePort",
		serviceName = "WebServiceInterface")
public class WebServiceImpl implements WebServiceInterface {

	private String original;
	private String updated;
	private Double amount;
	@Override
	public void print() {
		// TODO Auto-generated method stub
		//your own function implementation goes here
		System.out.printf("The amount converted is: %.2f", amount);
	}
	@Override
	public void setup(String o, String u, Double a) {
		original = o;
		updated = u;
		amount = a*Math.random();
	}

}
