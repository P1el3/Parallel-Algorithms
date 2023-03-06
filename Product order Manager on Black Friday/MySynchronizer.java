import java.io.FileWriter;
import java.io.IOException;

public class MySynchronizer {
	private static final MySynchronizer inst= new MySynchronizer();
	private FileWriter writer;

	private MySynchronizer() {
		super();
	}



	public synchronized void writeToFile(String str, String file) {
		try {
			this.writer = new FileWriter(file,true);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
		try {
			this.writer.write(str);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
		try {
			this.writer.close();
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}



	public static MySynchronizer getInstance() {
		return inst;
	}

}