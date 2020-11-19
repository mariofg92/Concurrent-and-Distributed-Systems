// fumadores2: 1 estanquero y 3 fumadores.
// A cada fumador le faltan dos ingredientes.

import monitor.*;
import java.util.Random;

class Estanco extends AbstractMonitor{
	private int estado = -1;	
	private Condition[] fumadores;
	private Condition estanquero = makeCondition();

	public Estanco(){
		fumadores = new Condition[3];
		fumadores[0] = makeCondition();
		fumadores[1] = makeCondition();
		fumadores[2] = makeCondition();
	}
	
	public void obtenerIngredientes (int ingrediente){
		enter();
		
		if (estado != ingrediente) 
			fumadores[ingrediente].await();
		
		System.out.println("El fumador " + estado + " está fumando");
		estado = -1;
		estanquero.signal();

		leave();
	} 

	public void ponerIngredientes(int ingrediente1, int ingrediente2){
		enter();

		if(estado == -1){
			if ((ingrediente1 == 0 && ingrediente2 ==1)||(ingrediente1 == 1 && ingrediente2 == 0))
				estado = 2;
			else if ((ingrediente1 == 0 && ingrediente2 ==2)||(ingrediente1 == 2 && ingrediente2 == 0))
				estado = 1;
			else if ((ingrediente1 == 1 && ingrediente2 ==2)||(ingrediente1 == 2 && ingrediente2 == 1))
				estado = 0;

			System.out.println("El fumador " + estado + " puede fumar");
			fumadores[estado].signal();
		}
		
		leave();
	} 

	public void esperarRecogidaIngredientes(){
		enter();

		if(estado != -1)
			estanquero.await();

		leave();
	}	
}	
	
class Fumador implements Runnable{
	private int ingrediente;
	private Estanco estanco;
	public Thread thr;

	public Fumador (Estanco estanco, int ingrediente, String nombre){
		this.ingrediente = ingrediente;
		this.estanco = estanco;
		thr = new Thread(this,nombre);
	}

	public void run(){
		while (true){
			estanco.obtenerIngredientes(ingrediente);
			aux.dormir_max(2000);		
		}
	}
}

class Estanquero implements Runnable{
	private Estanco estanco;
	public Thread thr;

	public Estanquero(Estanco estanco, String nombre){
		this.estanco = estanco;
		thr = new Thread(this,nombre);
	}
		
	public void run(){
		int ingrediente1, ingrediente2;
			
		while (true){
			do{
				ingrediente1 = (int) (Math.random () * 3.0);
				ingrediente2 = (int) (Math.random () * 3.0);	
			} while (ingrediente1 == ingrediente2);
			
			estanco.ponerIngredientes(ingrediente1, ingrediente2);
			estanco.esperarRecogidaIngredientes();
		}
	}
}

class aux{
	static Random genAlea = new Random();
	static void dormir_max(int milisecsMax){
		try{
			Thread.sleep(genAlea.nextInt(milisecsMax));		
		}
		catch(InterruptedException e){
			System.err.println("Sleep interrumpido en 'aux.dormir_max()'");
		}
	}
}

class EjemploFumadores2{
	public static void main(String[] args){
   	Estanco estanco = new Estanco();

		Estanquero estanquero = new Estanquero(estanco,"estanquero");
		Fumador fumador0 = new Fumador(estanco,0,"fumador 0");
		Fumador fumador1 = new Fumador(estanco,1,"fumador 1");
		Fumador fumador2 = new Fumador(estanco,2,"fumador 2");

		estanquero.thr.start();
		fumador0.thr.start();
		fumador1.thr.start();
		fumador2.thr.start();
	}
}
