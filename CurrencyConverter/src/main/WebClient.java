package main;
import java.net.MalformedURLException;
import java.net.URL;

import javax.xml.namespace.QName;
import javax.xml.ws.Service;

public class WebClient {

	public static void main(String[] args) throws MalformedURLException {
		URL url = new URL("http://localhost:8080/CurrencyConverter/WebServiceInterface?wsdl");
		QName qname = new QName("http://main/", "WebServiceInterface");
		Service service = Service.create(url, qname);
		WebServiceInterface callWebService = service.getPort(WebServiceInterface.class);
		System.out.println("Service output: \n");
		callWebService.setup(args[0], args[1], Double.parseDouble(args[2]));
		callWebService.print();
	}
}
