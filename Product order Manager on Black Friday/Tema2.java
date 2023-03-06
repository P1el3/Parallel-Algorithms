import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;


public class Tema2 {


	public static void main(String[] args) throws IOException, ExecutionException, InterruptedException {
		String directorName = args[0]; // file name
		int P = Integer.parseInt(args[1]); ; // max no of threads
		AtomicInteger curent_line = new AtomicInteger(0);
		//stuff from OCW

		ExecutorService order = Executors.newFixedThreadPool(P);
		ExecutorService product = Executors.newFixedThreadPool(P);

		//clean the files every time
		FileWriter fr_order = new FileWriter("order_products_out.txt");
		fr_order.write("");
		fr_order.close();
		FileWriter fr_product = new FileWriter("orders_out.txt");
		fr_product.write("");
		fr_product.close();
		//used to clean the output files for orders and products
		//in order to don't override them


		//making a thread for each parsed product
		List<Future<?>> threads = new ArrayList<>();
		for(int i = 0; i < P; i++)
			threads.add(order.submit(new Orders(i, P, directorName, product, curent_line)));

		for (var thread : threads)
			thread.get();


		//shuting down the executor services
		order.shutdown();
		product.shutdown();
	}
}
